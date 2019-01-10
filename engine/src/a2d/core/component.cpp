//
// Created by selya on 05.11.2018.
//

#include <a2d/core/component.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/commands/component_destroy_command.h>

namespace a2d {

Component::Component() {};

pObject2D Component::GetObject2D() const {
    return object_2d;
}

void Component::Destroy() {
    auto &components = object_2d->components;
    auto iter = components.find(typeid(*this));
    if (iter == components.end() || iter->second.empty()) return;
    auto comp_iter = std::find(iter->second.begin(), iter->second.end(), this);
    if (comp_iter == iter->second.end()) return;

    auto &drawables = object_2d->drawables;
    auto d = std::find(drawables.begin(), drawables.end(), this);
    if (d != drawables.end()) drawables.erase(d);
    
    iter->second.erase(comp_iter);

    Engine::AddCommand(new ComponentDestroyCommand(this));
}

} //namespace a2d


