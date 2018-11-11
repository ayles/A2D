//
// Created by selya on 26.10.2018.
//

#ifndef A2D_MACRO_H
#define A2D_MACRO_H

#include <A2D/core/intrusive_ptr.h>

#define DECLARE_SMART_POINTER(classname) \
class classname; \
typedef a2d::intrusive_ptr<classname> p##classname;

#define SMART_TYPE(classname) a2d::intrusive_ptr<classname>

#endif //A2D_MACRO_H
