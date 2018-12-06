//
// Created by selya on 05.11.2018.
//

#ifndef A2D_SPRITE_H
#define A2D_SPRITE_H

#include <a2d/core/drawable.h>
#include <a2d/math.h>
#include <a2d/core/texture.h>
#include <a2d/core/shader.h>

namespace a2d {

DECLARE_SMART_POINTER(Sprite)

class Sprite : public Drawable {
    friend class Renderer;
    friend class Object2D;

public:
    Vector4f color;
    pShader shader;

    const pTextureRegion &GetTextureRegion();

    const Shader *GetShaderForSortOrNull() const override;
    const TextureRegion *GetTextureRegionForSortOrNull() const override;

    void SetTextureRegion(const pTextureRegion &texture_region, bool pixel_size = true, bool adjust_size = true);

protected:
    pTextureRegion texture_region;

    Sprite();

    void Draw(SpriteBatch &sprite_batch) override;
    void OnEnable() override;
    void OnDisable() override;

    ~Sprite() override;
};

} //namespace a2d

#endif //A2D_SPRITE_H
