//
// Created by selya on 10.01.2019.
//

#ifndef A2D_LAMBDA_COMMAND_HPP
#define A2D_LAMBDA_COMMAND_HPP

#include <a2d/core/command.hpp>

#include <functional>
#include <utility>

namespace a2d {

class LambdaCommand : public Command {
    std::function<void()> lambda;

public:
    LambdaCommand(const std::function<void()> &lambda) : lambda(lambda) {}

    void Execute() override {
        lambda();
    }
};

} //namespace a2d

#endif //A2D_LAMBDA_COMMAND_HPP
