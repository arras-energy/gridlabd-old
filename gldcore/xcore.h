/* xcore.h
 *
 */

#ifndef _XCORE_H
#define _XCORE_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int xstart(void);

#ifdef __cplusplus
}
#endif

#endif
