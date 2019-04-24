//
// Created by selya on 26.10.2018.
//

#ifndef A2D_MACRO_HPP
#define A2D_MACRO_HPP

#include <a2d/core/intrusive_ptr.hpp>

#define DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(t)    \
t(const t &) = delete;                                  \
t(t &&) = delete;                                       \
t &operator=(const t &) = delete;                       \
t &operator=(t &&) = delete;

#define DECLARE_SMART_POINTER(classname) \
class classname; \
typedef a2d::intrusive_ptr<classname> p##classname;

#endif //A2D_MACRO_HPP
