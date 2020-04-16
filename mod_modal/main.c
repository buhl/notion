/*
 * ion/mod_modal/main.c
 *
 * Copyright (c) Tuomo Valkonen 2004-2007.
 *
 * See the included file LICENSE for details.
 */

#include <libextl/readconfig.h>
#include <ioncore/saveload.h>
#include <ioncore/bindmaps.h>

#include "exports.h"

/*{{{ Module information */


#include "../version.h"

char mod_modal_ion_api_version[]=ION_API_VERSION;


/*}}}*/


/*{{{ Bindmaps */


WBindmap *mod_modal_bindmap=NULL;


/*}}}*/


/*{{{ Init & deinit */


void mod_modal_deinit()
{
    if(mod_modal_bindmap!=NULL){
        ioncore_free_bindmap("WMgmtMode", mod_modal_bindmap);
        mod_modal_bindmap=NULL;
    }

    mod_modal_unregister_exports();
}


bool mod_modal_init()
{
    mod_modal_bindmap=ioncore_alloc_bindmap("WMgmtMode", NULL);

    if(mod_modal_bindmap==NULL)
        return FALSE;

    if(!mod_modal_register_exports()){
        mod_modal_deinit();
        return FALSE;
    }

    extl_read_config("cfg_modal", NULL, TRUE);

    return TRUE;
}


/*}}}*/

