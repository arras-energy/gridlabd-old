/** $Id: json.h 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
	@file json.h
	@addtogroup json
 @{
 **/

#ifndef _JSON_H
#define _JSON_H

#ifdef __cplusplus
extern "C" {
#endif

int json_dump(const char *filename);
int json_output(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif // _JSON_H
