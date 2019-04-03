/** $Id
 **/

#include <math.h>
#include "timestamp.h"
#include "property.h"
#include "object.h"
#include "compare.h"
#include "globals.h"

#define COMPAREOPI(T) COMPARE_EQI(T) COMPARE_LEI(T) COMPARE_GEI(T) COMPARE_NEI(T) COMPARE_LTI(T) COMPARE_GTI(T) COMPARE_INI(T) COMPARE_NII(T)
#define COMPARE_EQI(T) extern "C" int compare_tc_##T##_eq(void* x,void* a,void* b) { return (signed)*(T*)x==(signed)*(T*)a; }
#define COMPARE_LEI(T) extern "C" int compare_tc_##T##_le(void* x,void* a,void* b) { return (signed)*(T*)x<=(signed)*(T*)a; }
#define COMPARE_GEI(T) extern "C" int compare_tc_##T##_ge(void* x,void* a,void* b) { return (signed)*(T*)x>=(signed)*(T*)a; }
#define COMPARE_NEI(T) extern "C" int compare_tc_##T##_ne(void* x,void* a,void* b) { return (signed)*(T*)x!=(signed)*(T*)a; }
#define COMPARE_LTI(T) extern "C" int compare_tc_##T##_lt(void* x,void* a,void* b) { return (signed)*(T*)x<(signed)*(T*)a; }
#define COMPARE_GTI(T) extern "C" int compare_tc_##T##_gt(void* x,void* a,void* b) { return (signed)*(T*)x>(signed)*(T*)a; }
#define COMPARE_INI(T) extern "C" int compare_tc_##T##_in(void* x,void* a,void* b) { return (signed)*(T*)a<=(signed)*(T*)x && b!=NULL && (signed)*(T*)x<=(signed)*(T*)b; }
#define COMPARE_NII(T) extern "C" int compare_tc_##T##_ni(void* x,void* a,void* b) { return !((signed)*(T*)a<=(signed)*(T*)x && b!=NULL && (signed)*(T*)x<=(signed)*(T*)b); }

#define COMPAREOPB(T) COMPARE_EQB(T) COMPARE_NEB(T)
#define COMPARE_EQB(T) extern "C" int compare_tc_##T##_eq(void* x,void* a,void* b) { return *(T*)x==*(T*)a; }
#define COMPARE_NEB(T) extern "C" int compare_tc_##T##_ne(void* x,void* a,void* b) { return *(T*)x!=*(T*)a; }

#define COMPAREOPO(T) COMPARE_EQO(T) COMPARE_NEO(T)
//#define COMPARE_EQO(T) int compare_tc_##T##_eq(void* x,void* a,void* b) { return strcmp((*(OBJECT**)x)->name,(*(OBJECT**)a)->name)==0; }
//#define COMPARE_NEO(T) int compare_tc_##T##_ne(void* x,void* a,void* b) { return strcmp((*(OBJECT**)x)->name,(*(OBJECT**)a)->name)!=0; }
#define COMPARE_EQO(T) extern "C" int compare_tc_##T##_eq(void* x,void* a,void* b) { return (*(OBJECT**)x)==(*(OBJECT**)a); }
#define COMPARE_NEO(T) extern "C" int compare_tc_##T##_ne(void* x,void* a,void* b) { return (*(OBJECT**)x)!=(*(OBJECT**)a); }

#define COMPAREOPF(T) COMPARE_EQF(T) COMPARE_LEF(T) COMPARE_GEF(T) COMPARE_NEF(T) COMPARE_LTF(T) COMPARE_GTF(T) COMPARE_INF(T) COMPARE_NIF(T)
#define COMPARE_EQF(T) extern "C" int compare_tc_##T##_eq(void* x,void* a,void* b) { if (b==NULL) return *(T*)x==*(T*)a; else return fabs((*(T*)x)-(*(T*)a))<=(*(T*)b); }
#define COMPARE_LEF(T) extern "C" int compare_tc_##T##_le(void* x,void* a,void* b) { return *(T*)x<=*(T*)a; }
#define COMPARE_GEF(T) extern "C" int compare_tc_##T##_ge(void* x,void* a,void* b) { return *(T*)x>=*(T*)a; }
#define COMPARE_NEF(T) extern "C" int compare_tc_##T##_ne(void* x,void* a,void* b) { if (b==NULL) return *(T*)x!=*(T*)a; else return fabs((*(T*)x)-(*(T*)a))>(*(T*)b); }
#define COMPARE_LTF(T) extern "C" int compare_tc_##T##_lt(void* x,void* a,void* b) { return *(T*)x<*(T*)a; }
#define COMPARE_GTF(T) extern "C" int compare_tc_##T##_gt(void* x,void* a,void* b) { return *(T*)x>*(T*)a; }
#define COMPARE_INF(T) extern "C" int compare_tc_##T##_in(void* x,void* a,void* b) { return *(T*)a<=*(T*)x && b!=NULL && *(T*)x<=*(T*)b; }
#define COMPARE_NIF(T) extern "C" int compare_tc_##T##_ni(void* x,void* a,void* b) { return !(*(T*)a<=*(T*)x && b!=NULL && *(T*)x<=*(T*)b); }

#define COMPAREOPS(T) COMPARE_SEQ(T) COMPARE_SLE(T) COMPARE_SGE(T) COMPARE_SNE(T) COMPARE_SLT(T) COMPARE_SGT(T) COMPARE_SIN(T) COMPARE_SNI(T)
#define COMPARE_SEQ(T) extern "C" int compare_tc_##T##_eq(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)==0; }
#define COMPARE_SLE(T) extern "C" int compare_tc_##T##_le(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)!=1; }
#define COMPARE_SGE(T) extern "C" int compare_tc_##T##_ge(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)!=-1; }
#define COMPARE_SNE(T) extern "C" int compare_tc_##T##_ne(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)!=0; }
#define COMPARE_SLT(T) extern "C" int compare_tc_##T##_lt(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)==-1; }
#define COMPARE_SGT(T) extern "C" int compare_tc_##T##_gt(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)==1; }
#define COMPARE_SIN(T) extern "C" int compare_tc_##T##_in(void* x,void* a,void* b) { return strcmp((char*)x,(char*)a)!=-1 && b!=NULL && strcmp((char*)x,(char*)b)!=1; }
#define COMPARE_SNI(T) extern "C" int compare_tc_##T##_ni(void* x,void* a,void* b) { return !(strcmp((char*)x,(char*)a)!=-1 && b!=NULL && strcmp((char*)x,(char*)b)!=1); }

/* basic ops */
COMPAREOPF(double)
COMPAREOPF(float)
COMPAREOPI(uint16)
COMPAREOPI(uint32)
COMPAREOPI(uint64)
COMPAREOPB(bool)
COMPAREOPS(string)
COMPAREOPO(object)
