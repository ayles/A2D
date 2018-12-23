//
// Created by selya on 19.12.2018.
//

#ifndef A2D_PHYSICS_H
#define A2D_PHYSICS_H

#include <a2d/core/engine.h>
#include <a2d/core/object2d.h>

#include <Box2D/Box2D.h>

namespace a2d {

class Physics {
    friend class PhysicsBody;

public:
    static bool Initialize() { return true; }
    static void Uninitialize() {}

    static bool Step() {
        GetWorld().Step(Engine::GetDeltaTime(), 6, 2);
        Engine::GetRoot()->PhysicsUpdate();
        return true;
    }

private:
    static b2World &GetWorld() {
        static b2World world(b2Vec2(0.0f, -9.0f));
        return world;
    }
};

} //namespace a2d

#endif //A2D_PHYSICS_H
