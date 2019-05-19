/** $Id: kml.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file kml.h
	@addtogroup mapping
 @{
 **/

#ifndef KML_H
#define KML_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
int kml_dump(const char *filename);

#ifdef __cplusplus
}
#endif
	
#endif
