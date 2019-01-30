//
// Created by selya on 19.12.2018.
//

#ifndef A2D_PHYSICS_H
#define A2D_PHYSICS_H

#include <a2d/math/vector.hpp>
#include <a2d/core/intrusive_ptr.hpp>

#include <Box2D/Box2D.h>

#include <unordered_map>

namespace a2d {

class PhysicsCollider;

class CollisionContact;

class Physics {
    friend class Rigidbody;
    friend class NativeBridge;
    friend class ContactListener;
    friend class CollisionContact;

    static float world_scale;
    static float world_scale_inverted;
    static std::unordered_map<b2Contact *, intrusive_ptr<CollisionContact>> contacts;

public:
    static bool Initialize();
    static void Uninitialize();

    static float GetWorldScale();
    static Vector2f GetGravity();

    static void SetWorldScale(float world_scale);
    static void SetGravity(const Vector2f &gravity);
    static void SetGravity(float g_x, float g_y);

private:
    static bool Step();
    static b2World &GetWorld();

    static void BeginContact(b2Contact *contact);
    static void EndContact(b2Contact *contact);
    static void PreSolve(b2Contact *contact, const b2Manifold *old_manifold);
};

class CollisionContact : public ref_counter {
    friend class Physics;
    friend class ContactListener;

    b2Contact *contact;
    b2WorldManifold world_manifold;
    bool enabled;

    CollisionContact(b2Contact *contact) : contact(contact), enabled(contact->IsEnabled()) {}

public:
    intrusive_ptr<PhysicsCollider> GetColliderA() {
        return (PhysicsCollider*)contact->GetFixtureA()->GetUserData();
    }

    intrusive_ptr<PhysicsCollider> GetColliderB() {
        return (PhysicsCollider*)contact->GetFixtureB()->GetUserData();
    }

    Vector2f GetNormal() {
        contact->GetWorldManifold(&world_manifold);
        return Vector2f(world_manifold.normal.x, world_manifold.normal.y);
    }

    int GetPointCount() {
        return contact->GetManifold()->pointCount;
    }

    Vector2f GetPoint(int index) {
        contact->GetWorldManifold(&world_manifold);
        return Vector2f(world_manifold.points[index].x, world_manifold.points[index].y) *= Physics::world_scale_inverted;
    }

    bool IsEnabled() {
        return enabled;
    }

    bool IsTouching() {
        return contact->IsTouching();
    }

    void SetEnabled(bool enabled) {
        this->enabled = enabled;
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_H
