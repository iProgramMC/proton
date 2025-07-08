#ifndef PLATFORM_SETUP_WII_H
#define PLATFORM_SETUP_WII_H

#include <stdint.h>
#include <stdarg.h>

#include <GL/gl.h>

#ifndef M_PI
#define M_PI 3.141592f
#endif


#ifndef _SINT64
typedef int64_t sint64;
#define _SINT64
#endif

#ifndef _UINT64
typedef uint64_t uint64;
#define _UINT64
#endif

#ifndef HAVE_UINT64
 //need this to avoid mysql trying to redefine uint64
#define HAVE_UINT64
#endif


#ifndef _SINT32
typedef int32_t sint32;
#define _SINT32
#endif
#ifndef _SINT16
typedef int16_t sint16;
#define _SINT16
#endif
#ifndef _SINT8
typedef int8_t sint8;
#define _SINT8
#endif
#ifndef _UINT32
typedef uint32_t uint32;
#define _UINT32
#endif
#ifndef _UINT16
typedef uint16_t uint16;
#define _UINT16
#endif
#ifndef _UINT8
typedef uint8_t uint8;
#define _UINT8
#endif

#ifndef _INT32
typedef int32_t int32;
#define _INT32
#endif

#ifndef _INT16
typedef int16_t int16;
#define _INT16
#endif

#ifndef _INT8_DEFINED
typedef  signed char        int8;        /* Signed 8  bit value */
#define _INT8_DEFINED
#endif

#if defined(__GNUC__)
#define __int64 long long
#endif

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp

#ifndef _INT64_DEFINED
typedef  __int64            int64;       /* Signed 64 bit value */
#define _INT64_DEFINED
#endif

#ifndef _BYTE
typedef uint8_t uint8;
#define _BYTE
#endif


#define glTexParameterx glTexParameteri
#define glFrustumf glFrustum
#define glOrthof glOrtho
#define glColor4x(r,g,b,a) glColor4f( (float(r)/65536.0f),  (float(g)/65536.0f) , (float(b)/65536.0f), (float(a)/65536.0f));
//#define glActiveTexture glActiveTextureARB
//#define glClientActiveTexture glClientActiveTextureARB


#endif//PLATFORM_SETUP_WII_H