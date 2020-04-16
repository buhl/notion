/*
 * ion/mod_modal/modal.h
 *
 * Copyright (c) Tuomo Valkonen 2004-2007.
 *
 * See the included file LICENSE for details.
 */

#ifndef ION_MOD_MODAL_MODAL_H
#define ION_MOD_MODAL_MODAL_H

#include <ioncore/common.h>
#include <ioncore/log.h>
#include <libextl/extl.h>

INTRCLASS(WModal);

DECLCLASS(WModal){
    Obj obj;
    Watch selw;
};


extern WModal *mod_modal_begin(WRegion *reg);

extern void modal_select(WModal *mode, WRegion *reg);
extern WRegion *modal_selected(WModal *mode);

extern void modal_finish(WModal *mode);

#endif /* ION_MOD_MODAL_MODAL_H */
