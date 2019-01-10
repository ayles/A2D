//
// Created by selya on 10.01.2019.
//

#ifndef A2D_COMPONENT_ADD_COMMAND_H
#define A2D_COMPONENT_ADD_COMMAND_H

#include <a2d/core/engine.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/command.hpp>

#include <algorithm>

namespace a2d {

class ComponentAddCommand : public Command {
    Component *component;

public:
    ComponentAddCommand(const pComponent &component) : component(component.get()) {}

    void Execute() override {
        component->Initialize();
        if (Engine::IsPlaying()) component->OnResume();
        Engine::components.emplace_back(component);
    }
};

} //namespace a2d

#endif //A2D_COMPONENT_ADD_COMMAND_H
