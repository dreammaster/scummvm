         /* Copyright (c) 2010 by Michael J. Roberts.  All Rights Reserved. */
/*
Name
  vmnetui.cpp - TADS 3 networking, UI extensions
Function
  Implements UI-related routines for the TADS 3 networking subsystem.
Notes
  These routines are segregated from vmnet.cpp to untangle vmnet.cpp from
  the VM globals.  This allows that module to be used in build configurations
  that don't include the VM globals, such as for the front-end process for
  the stand-alone Web UI on Windows (tadsweb.exe).
Modified
  04/11/10 MJRoberts  - Creation
*/


#include <stdlib.h>
#include <string.h>

#include "glk/tads/tads3/t3std.h"
#include "glk/tads/os_glk.h"
#include "glk/tads/tads3/osifcnet.h"
#include "glk/tads/tads3/vmnet.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmerr.h"
#include "glk/tads/tads3/vmtype.h"
#include "glk/tads/tads3/vmobj.h"
#include "glk/tads/tads3/vmstr.h"
#include "glk/tads/tads3/vmlst.h"
#include "glk/tads/tads3/vmisaac.h"
#include "glk/tads/tads3/sha2.h"
#include "glk/tads/tads3/vmpredef.h"
#include "glk/tads/tads3/vmhttpreq.h"
#include "glk/tads/tads3/vmglob.h"
#include "glk/tads/tads3/vmmain.h"
#include "glk/tads/tads3/vmpredef.h"
#include "glk/tads/tads3/vmerrnum.h"
#include "glk/tads/tads3/vmrun.h"
#include "glk/tads/tads3/vmfile.h"


/* ------------------------------------------------------------------------ */
/*
 *   UI Close event - prepare the event object.
 */
vm_obj_id_t TadsUICloseEvent::prep_event_obj(VMG_ int *argc, int *evt_type)
{
    /* we're not adding any arguments */
    *argc = 0;

    /* the event type is NetEvUIClose (4 - see include/tadsnet.h) */
    *evt_type = 4;

    /* use the basic NetEvent class */
    return G_predef->net_event;
}
