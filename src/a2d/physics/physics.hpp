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
    friend class PhysicsCollider;

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
    intrusive_ptr<PhysicsCollider> GetColliderA();
    intrusive_ptr<PhysicsCollider> GetColliderB();
    Vector2f GetNormal();
    int GetPointCount();
    Vector2f GetPoint(int index);
    bool IsEnabled();
    bool IsTouching();
    void SetEnabled(bool enabled);
};

} //namespace a2d

#endif //A2D_PHYSICS_H
