////////////////////////////////////////////////////////////////////////////
// This contains the types and math classes that I'm used to work with.
// They have been stipped of most unneeded stuff that isn't needed in
// the screensaver
//
// Author:
//   Joakim Eriksson
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

/***************************** D E F I N E S *******************************/

typedef signed char     s8;
typedef unsigned char   u8;
typedef signed short    s16;
typedef unsigned short  u16;
typedef signed long     s32;
typedef unsigned long   u32;
typedef int64_t         s64;
typedef uint64_t        u64;
typedef float           f32;
typedef double          f64;
#define null 0

const f32 PI = 3.14159265358979323846f;
const f32 FLOATEPSILON = 0.00001f;

/****************************** M A C R O S ********************************/

#define SAFE_DELETE(_p) { if(_p) { delete _p; _p=nullptr; } }
#define SAFE_DELETE_ARRAY(_p) { if(_p) { delete [] _p; _p=nullptr; } }
#define SAFE_RELEASE(_p) { if(_p) { _p->Release();  _p=nullptr; } }

// Direct 3d verify
#define DVERIFY( _func ) \
  { \
    HRESULT _retCode = _func; \
    if( _retCode != D3D_OK) \
    { \
      char buf[2000]; \
      sprintf(buf, "\""#_func"\" returned 0x%lx in\n" __FILE__ "(%ld)\n", _retCode, __LINE__); \
      OutputDebugString(buf); \
      return false; \
    } \
  }

#define DEGTORAD(d)      ((d)*(PI / 180.0f))
#define RADTODEG(r)      ((r)*(180.0f / PI))
#define assert(_x)

/***************************** C L A S S E S *******************************/

////////////////////////////////////////////////////////////////////////////
//
class CRGBA
{
public:
  union
  {
    f32 col[4];
    struct
    {
      f32 r,g,b,a;
    };
  };

  CRGBA()
  {
    col[0] = col[1] = col[2] = 0.0f;
    col[3] = 1.0f;
  }

  CRGBA(f32 R, f32 G, f32 B, f32 A)
  {
    col[0]=R;
    col[1]=G;
    col[2]=B;
    col[3]=A;
  }

  void Set(f32 R, f32 G, f32 B, f32 A)
  {
    col[0]=R; col[1]=G; col[2]=B; col[3]=A;
  }

  CRGBA& operator = (const CRGBA& c)
  {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
  }
};

