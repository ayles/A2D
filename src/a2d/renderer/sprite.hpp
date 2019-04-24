//
// Created by selya on 05.11.2018.
//

#ifndef A2D_SPRITE_HPP
#define A2D_SPRITE_HPP

#include <a2d/renderer/drawable.hpp>
#include <a2d/math/math.hpp>
#include <a2d/renderer/texture/texture_region.hpp>
#include <a2d/renderer/shader.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Sprite)

class Sprite : public Drawable {
    friend class Object2D;

    Vector4f color;
public:
    void SetColor(const Vector4f &color);
    void SetColor(float r, float g, float b, float a);
    void SetTextureRegion(const pTextureRegion &texture_region) override;

    const Vector4f &GetColor();

protected:
    Sprite();
    ~Sprite() override;

    void Draw(SpriteBatch &sprite_batch, const Vector4f &color) override;
};

} //namespace a2d

#endif //A2D_SPRITE_HPP
