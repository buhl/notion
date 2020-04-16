/*
 * ion/mod_modal/modal.c
 *
 * Copyright (c) Tuomo Valkonen 2004-2007.
 *
 * See the included file LICENSE for details.
 */

#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#include <libtu/objp.h>
#include <ioncore/common.h>
#include <ioncore/global.h>
#include <ioncore/region.h>
#include <ioncore/rootwin.h>
#include <ioncore/binding.h>
#include <ioncore/grab.h>
#include "modal.h"
#include "main.h"


static WModal *modal_mode=NULL;


static void cancel_modal(WRegion *reg);


/*{{{ WModal */


static bool modal_init(WModal *mode, WRegion *reg)
{
    watch_init(&(mode->selw));
    watch_setup(&(mode->selw), (Obj*)reg, NULL);
    return TRUE;
}


static WModal *create_modal(WRegion *reg)
{
    CREATEOBJ_IMPL(WModal, modal, (p, reg));
}


static void modal_deinit(WModal *mode)
{
    if(modal_mode==mode)
       modal_mode=NULL;

    watch_reset(&(mode->selw));
}


/*EXTL_DOC
 * Select management mode target.
 */
EXTL_EXPORT_MEMBER
void modal_select(WModal *mode, WRegion *reg)
{
    watch_setup(&(mode->selw), (Obj*)reg, NULL);
}


/*EXTL_DOC
 * Return management mode target.
 */
EXTL_SAFE
EXTL_EXPORT_MEMBER
WRegion *modal_selected(WModal *mode)
{
    return (WRegion*)(mode->selw.obj);
}


/*EXTL_DOC
 * End management mode.
 */
EXTL_EXPORT_MEMBER
void modal_finish(WModal *mode)
{
    if(modal_mode==mode)
        cancel_modal(NULL);
}


EXTL_EXPORT
IMPLCLASS(WModal, Obj, modal_deinit, NULL);


/*}}}*/


/*{{{ Rubberband */


static void draw_rubberbox(WRootWin *rw, const WRectangle *rect)
{
    XPoint fpts[5];

    fpts[0].x=rect->x;
    fpts[0].y=rect->y;
    fpts[1].x=rect->x+rect->w;
    fpts[1].y=rect->y;
    fpts[2].x=rect->x+rect->w;
    fpts[2].y=rect->y+rect->h;
    fpts[3].x=rect->x;
    fpts[3].y=rect->y+rect->h;
    fpts[4].x=rect->x;
    fpts[4].y=rect->y;

    XDrawLines(ioncore_g.dpy, WROOTWIN_ROOT(rw), rw->xor_gc, fpts, 5,
               CoordModeOrigin);
}


static void modal_draw(WModal *mode)
{
    WRegion *reg=modal_selected(mode);

    if(reg!=NULL){
        WRootWin *rw=region_rootwin_of(reg);
        WRectangle g=REGION_GEOM(reg);
        int rx=0, ry=0;

        region_rootpos(reg, &rx, &ry);

        g.x=rx;
        g.y=ry;

        draw_rubberbox(rw, &g);
    }
}


static void modal_erase(WModal *mode)
{
    modal_draw(mode);
}


/*}}}*/


/*{{{ The mode */


static bool modal_handler(WRegion *reg, XEvent *xev)
{
    WRegion *mreg=NULL, *sub=NULL, *chld=NULL;
    XKeyEvent *ev=&xev->xkey;
    WBinding *binding=NULL;
    WModal *mode;

    if(ev->type==KeyRelease)
        return FALSE;

    if(reg==NULL)
        return FALSE;

    mode=modal_mode;

    if(mode==NULL)
        return FALSE;

    binding=bindmap_lookup_binding(mod_modal_bindmap,
                                   BINDING_KEYPRESS,
                                   ev->state, ev->keycode);

    if(!binding)
        return FALSE;

    if(binding!=NULL){
        modal_erase(mode);

        sub = (WRegion*) mode->selw.obj;
        mreg = sub;
        do{
            chld = mreg;
            if (chld!=NULL)
                LOG(INFO, GENERAL, "Loop at %s.", chld->ni.name);
            mreg=region_current(mreg);
        }while(mreg!=NULL);

        extl_call(binding->func, "ooo", NULL, mode, sub, chld);
        if(modal_mode!=NULL)
            modal_draw(modal_mode);
    }

    return (modal_mode==NULL);
}


static void cancel_modal(WRegion *reg)
{
    if(modal_mode!=NULL){
        modal_erase(modal_mode);
        destroy_obj((Obj*)modal_mode);
    }
    ioncore_grab_remove(modal_handler);
}


/*EXTL_DOC
 * Begin management mode.
 */
EXTL_EXPORT
WModal *mod_modal_begin(WRegion *reg)
{
    if(modal_mode!=NULL)
        return NULL;

    modal_mode=create_modal(reg);

    if(modal_mode==NULL)
        return NULL;

    ioncore_grab_establish((WRegion*)region_rootwin_of(reg),
                           modal_handler,
                           (GrabKilledHandler*)cancel_modal, 0,
                           GRAB_DEFAULT_FLAGS);

    modal_draw(modal_mode);

    return modal_mode;
}


/*}}}*/
