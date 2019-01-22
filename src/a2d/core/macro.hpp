//
// Created by selya on 26.10.2018.
//

#ifndef A2D_MACRO_H
#define A2D_MACRO_H

#include <a2d/core/intrusive_ptr.hpp>

#include <cassert>

#define DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(t)    \
t(const t &) = delete;                                  \
t(t &&) = delete;                                       \
t &operator=(const t &) = delete;                       \
t &operator=(t &&) = delete;

#define DECLARE_SMART_POINTER(classname) \
class classname; \
typedef a2d::intrusive_ptr<classname> p##classname;

//#define SMART_POINTER(classname) a2d::intrusive_ptr<classname>

#define ASSERT_MAIN_THREAD assert(std::this_thread::get_id() == a2d::Engine::GetMainThreadID());

#define ASSERT(i) assert(i);

#define DEBUG_ERROR(message) assert(false && (message));

#endif //A2D_MACRO_H
