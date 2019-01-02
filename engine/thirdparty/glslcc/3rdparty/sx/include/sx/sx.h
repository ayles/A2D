//
// Copyright 2018 Sepehr Taghdisian (septag@github). All rights reserved.
// License: https://github.com/septag/sx#license-bsd-2-clause
//
// sx.h - v1.0 - Main sx lib entry include file
//               Contains essential stdc includes and library definitions
//
#pragma once

#ifndef SX_SX_H_
#define SX_SX_H_

#include "config.h"

#include <alloca.h>         // alloca
#include <stdarg.h>         // va_list
#include <stdint.h>         // uint32_t
#include <stdbool.h>        // bool
#include <stdlib.h>         // size_t
#include <stddef.h>         // ptrdiff_t
#include <assert.h>         // assert

#include "platform.h"
#include "macros.h"

#if defined(__cplusplus)
#   if !defined(typeof)
#       define typeof(a) decltype(a)
#   endif
#else
// static_assert doesn't do anything in MSVC + C compiler, because we just don't have it !
#   ifndef static_assert
#       if SX_COMPILER_MSVC
#           define static_assert(_e, _msg)  
#       else
#           define static_assert(_e, _msg) _Static_assert(_e, _msg)
#       endif
#   endif
#endif

// Some libc function overrides
// Use sx_ versions in the code and override if required
#ifndef sx_assert
#   define sx_assert(_e)                assert(_e)
#endif

#ifndef sx_memset
#   include <string.h>         // memset
#   define sx_memset(_dst, _n, _sz)     memset(_dst, _n, _sz)
#endif

#ifndef sx_memcpy 
#   include <string.h>         // memcpy
#   define sx_memcpy(_dst, _src, _n)    memcpy(_dst, _src, _n)
#endif

#ifndef sx_memmove
#   include <string.h>         // memmove
#   define sx_memmove(_dst, _src, _n)   memmove(_dst, _src, _n)
#endif

#ifndef sx_memcmp
#   include <string.h>         // memcmp
#   define sx_memcmp(_p1, _p2, _n)      memcmp(_p1, _p2, _n)
#endif

#define sx_swap(a, b, _type)  do { _type tmp = a; a = b; b = tmp; } while (0)

#ifndef __cplusplus
#   if SX_COMPILER_GCC || SX_COMPILER_CLANG
#       define sx_max(a, b)  ({                 \
                typeof(a) __a = (a);            \
                typeof(b) __b = (b);            \
                (void) (&__a == &__b);          \
                __a > b ? __a : __b; })

#       define sx_min(a, b)  ({                 \
                typeof(a) __a = (a);            \
                typeof(b) __b = (b);            \
                (void) (&__a == &__b);          \
                __a < b ? __a : __b; })

#       define sx_clamp(v_, min_, max_) ({      \
                typeof(v_)   _v = (v_);         \
                typeof(min_) _min = (min_);     \
                typeof(max_) _max = (max_);     \
                (void) (&_min == &_max);        \
                _v = _v < _max ? _v : _max;     \
                _v > _min ? _v : _min; })
#   elif SX_COMPILER_MSVC
// NOTE: Because we have some features lacking in MSVC+C compiler, the max,min,clamp macros does not pre-evaluate the expressions
// So in performance critical code, make sure you re-evaluate the sx_max, sx_min, sx_clamp paramters before passing them to the macros
#           define sx_max(a, b)              ((a) > (b) ? (a) : (b))
#           define sx_min(a, b)              ((a) < (b) ? (a) : (b))
#           define sx_clamp(v, min_, max_)   sx_max(sx_min((v), (max_)), (min_))
#   endif  // SX_COMPILER_GCC||SX_COMPILER_CLANG
#else
template <typename T> T sx_max(T a, T b);
template <typename T> T sx_min(T a, T b);
template <typename T> T sx_clamp(T v, T _min, T _max);

template <> inline int sx_max(int a, int b)                {   return (a > b) ? a : b;  }
template <> inline float sx_max(float a, float b)          {   return (a > b) ? a : b;  }
template <> inline double sx_max(double a, double b)       {   return (a > b) ? a : b;  }
template <> inline int8_t sx_max(int8_t a, int8_t b)       {   return (a > b) ? a : b;  }
template <> inline uint8_t sx_max(uint8_t a, uint8_t b)    {   return (a > b) ? a : b;  }
template <> inline int16_t sx_max(int16_t a, int16_t b)    {   return (a > b) ? a : b;  }
template <> inline uint16_t sx_max(uint16_t a, uint16_t b) {   return (a > b) ? a : b;  }
template <> inline uint32_t sx_max(uint32_t a, uint32_t b) {   return (a > b) ? a : b;  }
template <> inline int64_t sx_max(int64_t a, int64_t b)    {   return (a > b) ? a : b;  }
template <> inline uint64_t sx_max(uint64_t a, uint64_t b) {   return (a > b) ? a : b;  }

template <> inline int sx_min(int a, int b)                {   return (a < b) ? a : b;  }
template <> inline float sx_min(float a, float b)          {   return (a < b) ? a : b;  }
template <> inline double sx_min(double a, double b)       {   return (a < b) ? a : b;  }
template <> inline int8_t sx_min(int8_t a, int8_t b)       {   return (a < b) ? a : b;  }
template <> inline uint8_t sx_min(uint8_t a, uint8_t b)    {   return (a < b) ? a : b;  }
template <> inline int16_t sx_min(int16_t a, int16_t b)    {   return (a < b) ? a : b;  }
template <> inline uint16_t sx_min(uint16_t a, uint16_t b) {   return (a < b) ? a : b;  }
template <> inline uint32_t sx_min(uint32_t a, uint32_t b) {   return (a < b) ? a : b;  }
template <> inline int64_t sx_min(int64_t a, int64_t b)    {   return (a < b) ? a : b;  }
template <> inline uint64_t sx_min(uint64_t a, uint64_t b) {   return (a < b) ? a : b;  }

template <> inline int sx_clamp(int v, int _min, int _max)                     {   return sx_max(sx_min(v, _max), _min);  }
template <> inline float sx_clamp(float v, float _min, float _max)             {   return sx_max(sx_min(v, _max), _min);  }
template <> inline double sx_clamp(double v, double _min, double _max)         {   return sx_max(sx_min(v, _max), _min);  }
template <> inline int8_t sx_clamp(int8_t v, int8_t _min, int8_t _max)         {   return sx_max(sx_min(v, _max), _min);  }
template <> inline uint8_t sx_clamp(uint8_t v, uint8_t _min, uint8_t _max)     {   return sx_max(sx_min(v, _max), _min);  }
template <> inline int16_t sx_clamp(int16_t v, int16_t _min, int16_t _max)     {   return sx_max(sx_min(v, _max), _min);  }
template <> inline uint16_t sx_clamp(uint16_t v, uint16_t _min, uint16_t _max) {   return sx_max(sx_min(v, _max), _min);  }
template <> inline uint32_t sx_clamp(uint32_t v, uint32_t _min, uint32_t _max) {   return sx_max(sx_min(v, _max), _min);  }
template <> inline int64_t sx_clamp(int64_t v, int64_t _min, int64_t _max)     {   return sx_max(sx_min(v, _max), _min);  }
template <> inline uint64_t sx_clamp(uint64_t v, uint64_t _min, uint64_t _max) {   return sx_max(sx_min(v, _max), _min);  }

#endif  

#endif  // SX_SX_H_

