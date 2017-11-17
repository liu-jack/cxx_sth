#pragma once

#include "def/TypeDef.h"

#define gll_new new
#define gll_delete delete

// OS .h
#ifdef WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#include <winsock2.h> // <winsock2.h> should be included in front of <windows.h>
	#include <windows.h>
	#include <time.h>
	#define snprintf _snprintf
#else
	#include <netinet/in.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <sys/timeb.h>
	#include <syslog.h>
	#include <unistd.h>
#endif


//the strength of gravity
#define ACTOR_GRAVITY (40.0f)
#define GRAVITY (9.81f)

#define MAX_FPS_2 30
#define FIXED_UPDATE_TIME (1000.f / MAX_FPS_2)

#define COUNT_OF(a) (sizeof(a)/sizeof(a[0]))
#define INT_MAXVALUE		0x7fffffff

#define PI	3.14159265359f
#define PI64 3.1415926535897932384626433832795028841971693993751

#define RECIPROCAL_PI64 (1.0/PI64)
#define DEGTORAD (PI/180.0f)
#define RADTODEG (180.0f/PI)
#define DEGTORAD64 (PI64/180.0)
#define RADTODEG64 (180.0/PI64)
#define	RAD_TO_DEG(x)					((x)*RADTODEG)
#define	DEG_TO_RAD(x)					((x)*DEGTORAD)



#define NORMALIZE(x, max)               { while ((x) >= (max)) (x) -= (max); while ((x) < 0) (x) += (max); }
#ifndef ABS
#define ABS(a)                          ((a) < 0 ? -(a) : (a))
#endif
#ifndef SGN
#define SGN(a)							(a < 0 ? -1 : 1)
#endif
#ifndef MAX
#define MAX(a, b)						((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b)						((a) < (b) ? (a) : (b))
#endif

#define CLAMP(min, x, max)				{ (x) = MAX((x), (min)); (x) = MIN((x), (max)); }
#define SQUARE(x)			            ((x)*(x))
#define FLOAT_EPSINON					0.000001
#define FLOAT_EQUALS_ZERO(a)			((a) > - FLOAT_EPSINON && (a) < FLOAT_EPSINON)

#define CM2M							(0.01f)

#define XTOF(x)							((float)((x)*(1.0f/65536.0f)))
#define FTOX(x)							((GLfixed)((x)*(65536.0f)))

#define SAFE_FREE(a)					{ if(a) {free(a); a = NULL;} }
#define SAFE_DEL(a)						{ if(a) {delete a; a = NULL;} }
#define SAFE_DEL_ARRAY(a)				{ if(a) {delete [](a); a = NULL;} }
#define SAFE_DEL_STRING_ARRAY(a, len)	{ \
	if (a) \
{ \
	for (int __iii1 = 0; __iii1 < len; __iii1++) \
{ \
	SAFE_DEL_ARRAY(a[__iii1]); \
} \
	delete [](a); \
	a = NULL; \
} \
}

#define _GET_U32(array) (( array[off] ) | ( (array[off + 1]) << 8 ) | ( (array[off + 2]) << 16 ) | ( (array[off + 3]) << 24 ))
#define _GET_U16(array) (( array[off] ) | ( (array[off + 1]) << 8 ))
#define _GET_U8(array)  (( array[off] ))

#define GET_U32(array) (U32)_GET_U32(array); off+=4;
#define GET_U16(array) (U16)_GET_U16(array); off+=2;
#define GET_U8(array)  (U8)_GET_U8(array); off+=1;

#define GET_S32(array) (S32)_GET_U32(array); off+=4;
#define GET_S16(array) (S16)_GET_U16(array); off+=2;
#define GET_S8(array)  (S8)_GET_U8(array); off+=1;

#ifndef FLT_MIN
#define FLT_MIN __FLT_MIN__
#endif
#ifndef FLT_MAX
#define FLT_MAX __FLT_MAX__
#endif

#ifdef WIN32
#define STRICMP _stricmp
#define STRDUP _strdup

#define ISVALIDFLOAT(x) (_isnan(x) == 0)
//#define SPRINTF sprintf_s
//#define STRCPY strcpy_s
#else
#define STRICMP stricmp
#define STRDUP strdup
#define ISVALIDFLOAT(x) ( isnan(x) == 0 )
//#define SPRINTF sprintf_s
//#define STRCPY strcpy_s
#endif

const F32 ROUNDING_ERROR_32 = 0.00005f;
const F64 ROUNDING_ERROR_64 = 0.000005;
#define F32_VALUE_0		0x00000000
#define F32_VALUE_1		0x3f800000
#define F32_SIGN_BIT		0x80000000U
#define F32_EXPON_MANTISSA	0x7FFFFFFFU

//! code is taken from IceFPU
//! Integer representation of a floating-point value.
#define IR(x)				((u32&)(x))

//! Absolute integer representation of a floating-point value
#define AIR(x)				(IR(x)&0x7fffffff)

//! Floating-point representation of an integer value.
#define FR(x)				((f32&)(x))

//! integer representation of 1.0
#define IEEE_1_0			0x3f800000
//! integer representation of 255.0
#define IEEE_255_0			0x437f0000

#ifndef MAXUINT32
#define MAXUINT32   ((uint32)~((uint32)0))
#endif

class CResFileStream;
typedef CResFileStream IFileStream;

#include "def/MmoAssert.h"

typedef uint64	StreamSize;
#define GLITCH_DEBUG_BREAK_IF(cond)  MMO_ASSERT(!(cond))

#include "VEC3.h"
#include "AABB.h"
#include "MTX4.h"

#ifndef MMO_OFFSET
#define MMO_OFFSET(s,m)   (size_t)&(((s *)0)->m)
#endif

#ifndef ZeroMemoryThis
#define ZeroMemoryThis   memset(this, 0, sizeof(*this))
#endif
#ifndef ZeroMemoryArray
#define ZeroMemoryArray(arr)   memset(arr, 0, sizeof(arr))
#endif

template <typename T> int SUM_ARR(T* arr, int size){
    int sum(0);
    for (int j = 0; j < size; ++j) sum += arr[j];
    return sum;
}
template <typename T> bool IS_IN_ARR(T* arr, int size, T& val){
    for (int j = 0; j < size; ++j)
        if (arr[j] == val) return true;
    return false;
}