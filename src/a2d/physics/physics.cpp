//
// Created by selya on 28.01.2019.
//

#include <a2d/physics/physics.hpp>
#include <a2d/components/physics/rigidbody.hpp>


namespace a2d {

float Physics::world_scale = 0.01f;
float Physics::world_scale_inverted = 1 / world_scale;

bool Physics::Initialize() {
    return true;
}

void Physics::Uninitialize() {}

bool Physics::Step() {
    for (auto &component : Engine::components) {
        component->PhysicsUpdate();
    }
    GetWorld().Step(Engine::GetDeltaTime(), 6, 2);
    auto b = GetWorld().GetBodyList();
    while (b) {
        ((Rigidbody *)b->GetUserData())->PhysicsStep();
        b = b->GetNext();
    }
    Engine::ExecuteCommands();
    return true;
}

b2World& Physics::GetWorld() {
    static b2World world(b2Vec2(0.0f, -9.8f));
    return world;
}

float Physics::GetWorldScale() {
    return world_scale;
}

void Physics::SetWorldScale(float world_scale) {
    if (GetWorld().GetBodyCount()) {
        LOG_TRACE("Can't change physics world scale when there is bodies");
        return;
    }
    Physics::world_scale = world_scale;
    Physics::world_scale_inverted = 1 / world_scale;
}

}
