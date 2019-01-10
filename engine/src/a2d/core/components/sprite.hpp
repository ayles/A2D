//
// Created by selya on 05.11.2018.
//

#ifndef A2D_SPRITE_H
#define A2D_SPRITE_H

#include <a2d/graphics/drawable.hpp>
#include <a2d/math/math.hpp>
#include <a2d/graphics/texture_region.hpp>
#include <a2d/graphics/shader.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Sprite)

class Sprite : public Drawable {
    friend class Object2D;

public:
    Vector4f color;
    pShader shader;

    const pTextureRegion &GetTextureRegion();

    const Shader *GetShaderForSortOrNull() const override;
    const TextureRegion *GetTextureRegionForSortOrNull() const override;

    float GetScaleFactor() const;

    void SetScaleFactor(float scale_factor);
    void SetTextureRegion(const pTextureRegion &texture_region, float scale_factor = 0.0f);

protected:
    pTextureRegion texture_region;
    float scale_factor;

    Sprite();
    ~Sprite() override;

    void Draw(SpriteBatch &sprite_batch) override;
};

} //namespace a2d

#endif //A2D_SPRITE_H
