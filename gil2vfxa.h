//#include "aesop/vfx.h"
#include "aesop/defs.h"
#include "aesop/stubs.h"

#ifndef AESOP_GIL2VFXA_H
#define AESOP_GIL2VFXA_H

namespace Aesop {

extern void GIL2VFXA_print_buffer(PANE *pane, LONG line_number);
extern void GIL2VFX_hook_INT9(void);
extern void GIL2VFX_unhook_INT9(void);

}

#endif
