//
// Created by selya on 05.11.2018.
//

#include <a2d/core/component.hpp>
#include <a2d/core/object2d.hpp>

namespace a2d {

Component::Component() : initialized(false), active(false), active_transitive(false) {
    ASSERT_MAIN_THREAD
};

pObject2D Component::GetObject2D() const {
    ASSERT_MAIN_THREAD
    return object_2d;
}

void Component::Destroy() {
    ASSERT_MAIN_THREAD
    Engine::AddCommand([this]() {
        auto iter = object_2d->components.find(typeid(*this));
        if (iter == object_2d->components.end() || iter->second.empty()) return;
        if (Engine::IsPlaying()) this->OnPause();
        this->SetActive(false);
        this->OnDetach();
        this->OnDestroy();
        iter->second.erase(this);
        Engine::components.erase(this);
    });
}

void Component::SetActive(bool active) {
    this->active = active;
    bool old_active_transitive = this->active_transitive;
    this->active_transitive = GetObject2D()->active_transitive && active;
    if (old_active_transitive != this->active_transitive) {
        if (this->active_transitive) OnEnable();
        else OnDisable();
    }
}

bool Component::IsActive() const {
    return active;
}

bool Component::IsActiveTransitive() const {
    return active_transitive;
}

} //namespace a2d


