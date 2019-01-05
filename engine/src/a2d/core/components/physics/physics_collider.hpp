//
// Created by selya on 25.12.2018.
//

#ifndef A2D_PHYSICS_COLLIDER_H
#define A2D_PHYSICS_COLLIDER_H

#include <a2d/core/component.hpp>
#include <a2d/core/object2d.hpp>
#include <a2d/core/components/physics/physics_body.hpp>

namespace a2d {

class PhysicsBody;

DECLARE_SMART_POINTER(PhysicsCollider)

class PhysicsCollider : public Component {

protected:
    SMART_POINTER(PhysicsBody) body = nullptr;

    virtual void AttachToBody(const SMART_POINTER(PhysicsBody) &body) {
        this->body = body;
    }

    virtual void DetachFromBody(const SMART_POINTER(PhysicsBody) &body) {
        this->body = nullptr;
    }

    void FindAndAttach() {
        auto o = GetObject2D();

        do {
            auto c = o->GetComponent<PhysicsBody>();
            if (c) {
                AttachToBody(c);
                break;
            }
            o = o->GetParent();
        } while (o);
    }

    void Initialize() override {
        FindAndAttach();
    }

    void PhysicsUpdate() override {
        if (!body) FindAndAttach();
        else {
            // TODO fix this by checking transform changes
            auto b = body;
            DetachFromBody(b);
            AttachToBody(b);
        }
    }

    void OnDestroy() override {
        if (body) DetachFromBody(body);
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_COLLIDER_H
