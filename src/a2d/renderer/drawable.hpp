//
// Created by selya on 24.11.2018.
//

#ifndef A2D_DRAWABLE_H
#define A2D_DRAWABLE_H

#include <a2d/core/component.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/renderer/sprite_batch.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/renderer/texture_region.hpp>
#include <a2d/renderer/material.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Drawable)

class Drawable : public Component {
    friend class Object2D;
public:

    Drawable();
    ~Drawable() override = 0;

    /**
     * Returns width of this drawable (not accounting for Object2D scale).
     *
     * @return width
     */
    virtual float GetWidth() const final;

    /**
     * Returns height of this drawable (not accounting for Object2D scale).
     *
     * @return height
     */
    virtual float GetHeight() const final;

    /**
     * Returns size of this drawable (width, height).
     *
     * @return size (width, height)
     */
    virtual Vector2f GetSize() const final;

    /**
     * Returns origin of this drawable.
     *
     * (0, 0) - bottom-left corner
     * (1, 1) - top-right corner
     * Origin specifies which point will be center point of this drawable.
     *
     * @return origin
     */
    virtual Vector2f GetOrigin() const final;

    virtual const pTextureRegion GetTextureRegion() const final;
    virtual const pMaterial GetMaterial() const final;

    virtual void SetWidth(float width);
    virtual void SetHeight(float height);
    virtual void SetSize(float x, float y);
    virtual void SetSize(const Vector2f &size);
    virtual void SetOrigin(float x, float y);
    virtual void SetOrigin(const Vector2f &origin);
    virtual void SetTextureRegion(const pTextureRegion &texture_region);
    virtual void SetMaterial(const pMaterial &material);


    /**
     * Compares drawables for minimizing shader/texture switches.
     */
    virtual bool IsLess(const Drawable &other) const final;

protected:
    Vector2f size;
    Vector2f origin;
    pTextureRegion texture_region;
    pMaterial material;

    virtual void Draw(SpriteBatch &sprite_batch);

    void Initialize() override;
    void OnDestroy() override;
};

} //namespace a2d

#endif //A2D_DRAWABLE_H
