#line 30 "pdlsimple.h.PL"

#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#ifndef __PDL_H

/* These are kept automatically in sync with pdl.h during perl build */
#define PDL_TYPELIST_ALL(X) \
 X(PDL_SB,PDL_SByte,A,SByte,SCHAR_MIN,signed char)\
 X(PDL_B,PDL_Byte,B,Byte,UCHAR_MAX,unsigned char)\
 X(PDL_S,PDL_Short,S,Short,SHRT_MIN,short)\
 X(PDL_US,PDL_Ushort,U,Ushort,USHRT_MAX,unsigned short)\
 X(PDL_L,PDL_Long,L,Long,INT32_MIN,int32_t)\
 X(PDL_UL,PDL_ULong,K,ULong,UINT32_MAX,uint32_t)\
 X(PDL_IND,PDL_Indx,N,Indx,PTRDIFF_MIN,ptrdiff_t)\
 X(PDL_ULL,PDL_ULongLong,P,ULongLong,UINT64_MAX,uint64_t)\
 X(PDL_LL,PDL_LongLong,Q,LongLong,INT64_MIN,int64_t)\
 X(PDL_F,PDL_Float,F,Float,-FLT_MAX,float)\
 X(PDL_D,PDL_Double,D,Double,-DBL_MAX,double)\
 X(PDL_LD,PDL_LDouble,E,LDouble,-LDBL_MAX,long double)\
 X(PDL_CF,PDL_CFloat,G,CFloat,(-FLT_MAX - I*FLT_MAX),complex float)\
 X(PDL_CD,PDL_CDouble,C,CDouble,(-DBL_MAX - I*DBL_MAX),complex double)\
 X(PDL_CLD,PDL_CLDouble,H,CLDouble,(-LDBL_MAX - I*LDBL_MAX),complex long double)\


#line 55 "pdlsimple.h.PL"

#define X(sym, ...) \
  , sym
typedef enum {
   PDL_INVALID=-1
PDL_TYPELIST_ALL(X)
} pdl_datatypes;
#undef X

#define X(sym, ctype, ppsym, shortctype, defbval, realctype, ...) \
  typedef realctype ctype;
PDL_TYPELIST_ALL(X)
#undef X

#endif

/*
   Define a simple pdl C data structure which maps onto passed
   ndarrays for passing with callext().

   Note it is up to the user at the perl level to get the datatype
   right. Anything more sophisticated probably ought to go through
   PP anyway (which is fairly trivial).
*/

typedef struct {
   pdl_datatypes datatype;  /* whether byte/int/float etc. */
   void       *data;  /* Generic pointer to the data block */
   PDL_Indx  nvals;  /* Number of data values */
   PDL_Indx  *dims;  /* Array of data dimensions */
   PDL_Indx  ndims;  /* Number of data dimensions */
} pdlsimple;
