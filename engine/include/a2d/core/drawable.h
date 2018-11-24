//
// Created by selya on 24.11.2018.
//

#ifndef A2D_DRAWABLE_H
#define A2D_DRAWABLE_H

#include <a2d/core/component.h>
#include <a2d/core/macro.h>
#include <a2d/core/sprite_batch.h>

namespace a2d {

DECLARE_SMART_POINTER(Drawable)

class Drawable : public Component {
public:
    virtual void Draw(SpriteBatch &sprite_batch) = 0;
    virtual bool operator<(const Drawable &other) {
        return this < &other;
    }
};

} //namespace a2d

#endif //A2D_DRAWABLE_H
