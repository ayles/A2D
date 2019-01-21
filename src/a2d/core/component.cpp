//
// Created by selya on 05.11.2018.
//

#include <a2d/core/component.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Component::Component() {};

pObject2D Component::GetObject2D() const {
    return object_2d;
}

void Component::Destroy() {
    Engine::AddCommand([this]() {
        auto iter = object_2d->components.find(typeid(*this));
        if (iter == object_2d->components.end() || iter->second.empty()) return;
        iter->second.erase(this);
        if (Engine::IsPlaying()) this->OnPause();
        this->OnDestroy();
        Engine::components.erase(this);
    });
}

} //namespace a2d


