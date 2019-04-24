//
// Created by selya on 09.01.2019.
//

#ifndef A2D_COMMAND_HPP
#define A2D_COMMAND_HPP

#include <a2d/core/macro.hpp>
#include <a2d/core/ref_counter.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Command)

class Command : public ref_counter {
public:
    virtual void Execute() = 0;
};

} //namespace a2d

#endif //A2D_COMMAND_HPP
