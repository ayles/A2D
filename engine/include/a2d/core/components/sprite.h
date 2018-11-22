//
// Created by selya on 05.11.2018.
//

#ifndef A2D_SPRITE_H
#define A2D_SPRITE_H

#include <a2d/core/component.h>
#include <a2d/math.h>
#include <a2d/core/texture.h>
#include <a2d/core/shader.h>

namespace a2d {

DECLARE_SMART_POINTER(Sprite)

class Sprite : public Component {
    friend class Renderer;
    friend class Object2D;

public:
    Vector4f color;
    pShader shader;

    const pTextureRegion &GetTextureRegion();

    void SetTextureRegion(const pTextureRegion &texture_region);

private:
    pTextureRegion texture_region;

    Sprite();

    void OnEnable() override;
    void OnDisable() override;
    void LayerHierarchyAboutToChange() override;
    void LayerHierarchyChanged() override;

    ~Sprite() override;
};

} //namespace a2d

#endif //A2D_SPRITE_H
