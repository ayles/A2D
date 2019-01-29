//
// Created by selya on 19.12.2018.
//

#ifndef A2D_PHYSICS_H
#define A2D_PHYSICS_H

#include <a2d/core/engine.hpp>
#include <a2d/core/object2d.hpp>

#include <Box2D/Box2D.h>

#include <set>

namespace a2d {

class Physics {
    friend class Rigidbody;
    friend class NativeBridge;

    static float world_scale;
    static float world_scale_inverted;
public:

    static bool Initialize();
    static void Uninitialize();

    static float GetWorldScale();
    static void SetWorldScale(float world_scale);

private:
    static bool Step();
    static b2World &GetWorld();
};

} //namespace a2d

#endif //A2D_PHYSICS_H
