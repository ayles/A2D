//
// Created by selya on 28.01.2019.
//

#include <a2d/physics/physics.hpp>
#include <a2d/physics/rigidbody.hpp>

#include <unordered_map>


namespace a2d {

float Physics::world_scale = 0.01f;
float Physics::world_scale_inverted = 1 / world_scale;
std::unordered_map<b2Contact *, intrusive_ptr<CollisionContact>> Physics::contacts;

class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact *contact) override {
        Physics::BeginContact(contact);
    }

    void EndContact(b2Contact *contact) override {
        Physics::EndContact(contact);
    }

    void PreSolve(b2Contact *contact, const b2Manifold *old_manifold) override {
        Physics::PreSolve(contact, old_manifold);
    }
};

intrusive_ptr<PhysicsCollider> CollisionContact::GetColliderA() {
    return (PhysicsCollider*)contact->GetFixtureA()->GetUserData();
}

intrusive_ptr<PhysicsCollider> CollisionContact::GetColliderB() {
    return (PhysicsCollider*)contact->GetFixtureB()->GetUserData();
}

Vector2f CollisionContact::GetNormal() {
    contact->GetWorldManifold(&world_manifold);
    return Vector2f(world_manifold.normal.x, world_manifold.normal.y);
}

int CollisionContact::GetPointCount() {
    return contact->GetManifold()->pointCount;
}

Vector2f CollisionContact::GetPoint(int index) {
    contact->GetWorldManifold(&world_manifold);
    return Vector2f(world_manifold.points[index].x, world_manifold.points[index].y) *= Physics::world_scale_inverted;
}

bool CollisionContact::IsEnabled() {
    return enabled;
}

bool CollisionContact::IsTouching() {
    return contact->IsTouching();
}

void CollisionContact::SetEnabled(bool enabled) {
    this->enabled = enabled;
}

bool Physics::Initialize() {
    GetWorld().SetContactListener(new ContactListener);
    return true;
}

void Physics::Uninitialize() {}

bool Physics::Step() {
    for (auto &component : Engine::components) {
        component->PhysicsUpdate();
    }
    Engine::ExecuteCommands();
    GetWorld().Step(Engine::GetDeltaTime(), 6, 2);
    auto b = GetWorld().GetBodyList();
    while (b) {
        auto rigidbody = (Rigidbody *)b->GetUserData();
        if (rigidbody->IsActiveTransitive()) rigidbody->PhysicsStep();
        b = b->GetNext();
    }
    return true;
}

b2World &Physics::GetWorld() {
    static b2World world(b2Vec2(0.0f, 0.0f));
    return world;
}

void Physics::BeginContact(b2Contact *contact) {
    intrusive_ptr<CollisionContact> collision_contact = new CollisionContact(contact);
    contacts.emplace(contact, collision_contact);
    for (auto &c : ((Rigidbody *)contact->GetFixtureA()->GetBody()->GetUserData())->GetObject2D()->components) {
        for (auto &component : c.second) {
            component->OnCollisionBegin(collision_contact);
        }
    }
    for (auto &c : ((Rigidbody *)contact->GetFixtureB()->GetBody()->GetUserData())->GetObject2D()->components) {
        for (auto &component : c.second) {
            component->OnCollisionBegin(collision_contact);
        }
    }
}

void Physics::EndContact(b2Contact *contact) {
    auto i = contacts.find(contact);
    intrusive_ptr<CollisionContact> collision_contact;
    if (i != contacts.end()) {
        collision_contact = i->second;
        contacts.erase(i);
    }
    for (auto &c : ((Rigidbody *)contact->GetFixtureA()->GetBody()->GetUserData())->GetObject2D()->components) {
        for (auto &component : c.second) {
            component->OnCollisionEnd(collision_contact);
        }
    }
    for (auto &c : ((Rigidbody *)contact->GetFixtureB()->GetBody()->GetUserData())->GetObject2D()->components) {
        for (auto &component : c.second) {
            component->OnCollisionEnd(collision_contact);
        }
    }
}

void Physics::PreSolve(b2Contact *contact, const b2Manifold *old_manifold) {
    auto i = contacts.find(contact);
    if (i != contacts.end()) {
        // Set enable status
        contact->SetEnabled(i->second->enabled);
    }
}

float Physics::GetWorldScale() {
    return world_scale;
}

void Physics::SetWorldScale(float world_scale) {
    if (GetWorld().GetBodyCount()) {
        LOG_TRACE("Can't change physics world scale when there is bodies");
        return;
    }
    b2Vec2 gravity = GetWorld().GetGravity();
    gravity *= world_scale_inverted;
    Physics::world_scale = world_scale;
    Physics::world_scale_inverted = 1 / world_scale;
    gravity *= world_scale;
    GetWorld().SetGravity(gravity);
}

Vector2f Physics::GetGravity() {
    return Vector2f(GetWorld().GetGravity().x, GetWorld().GetGravity().y) *= world_scale_inverted;
}

void Physics::SetGravity(const Vector2f &gravity) {
    SetGravity(gravity.x, gravity.y);
}

void Physics::SetGravity(float g_x, float g_y) {
    GetWorld().SetGravity(b2Vec2(g_x * world_scale, g_y * world_scale));
}

}
