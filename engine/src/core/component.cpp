//
// Created by selya on 05.11.2018.
//

#include <a2d/core/component.h>
#include <a2d/core/object2d.h>

a2d::Component::Component() : is_active(false) {

}

a2d::pObject2D a2d::Component::GetObject2D() {
    return object_2d;
}

bool a2d::Component::IsActive() {
    return is_active;
}

void a2d::Component::SetActive(bool active) {
    if (active != is_active) {
        is_active = active;
        if (is_active) OnEnable();
        else OnDisable();
    }
}



