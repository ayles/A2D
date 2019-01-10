//
// Created by selya on 10.01.2019.
//

#ifndef A2D_COMPONENT_DESTROY_COMMAND_H
#define A2D_COMPONENT_DESTROY_COMMAND_H

#include <a2d/core/object2d.hpp>
#include <a2d/core/component.hpp>
#include <a2d/core/command.hpp>

#include <algorithm>

namespace a2d {

class ComponentDestroyCommand : public Command {
    Component *component;

public:
    ComponentDestroyCommand(const pComponent &component) : component(component.get()) {}

    void Execute() override {
        if (Engine::IsPlaying()) component->OnPause();
        component->OnDestroy();
        Engine::components.erase(std::find(Engine::components.begin(), Engine::components.end(), component));
    }
};

} //namespace a2d

#endif //A2D_COMPONENT_DESTROY_COMMAND_H
