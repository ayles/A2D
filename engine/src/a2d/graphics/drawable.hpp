//
// Created by selya on 24.11.2018.
//

#ifndef A2D_DRAWABLE_H
#define A2D_DRAWABLE_H

#include <a2d/core/component.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/graphics/sprite_batch.hpp>
#include <a2d/math/vector.hpp>
#include <a2d/graphics/shader.hpp>
#include <a2d/graphics/texture_region.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Drawable)

class Drawable : public Component {
    friend class Object2D;
public:

    /**
     * Returns width of this drawable (not accounting for Object2D scale).
     *
     * @return width
     */
    virtual float GetWidth() const;

    /**
     * Returns height of this drawable (not accounting for Object2D scale).
     *
     * @return height
     */
    virtual float GetHeight() const;

    /**
     * Returns size of this drawable (width, height).
     *
     * @return size (width, height)
     */
    virtual Vector2f GetSize() const;

    /**
     * Returns origin of this drawable.
     *
     * (0, 0) - bottom-left corner
     * (1, 1) - top-right corner
     * Origin specifies which point will be center point of this drawable.
     *
     * @return origin
     */
    virtual Vector2f GetOrigin() const;

    virtual void SetWidth(float width);
    virtual void SetHeight(float height);
    virtual void SetSize(float x, float y);
    virtual void SetSize(const Vector2f &size);
    virtual void SetOrigin(float x, float y);
    virtual void SetOrigin(const Vector2f &origin);

    /**
     * Compares drawables for minimizing shader/texture switches.
     */
    virtual bool operator<(const Drawable &other) const;

    ~Drawable() override = 0;

protected:
    Vector2f size;
    Vector2f origin;

    /**
     * Called for setting next frame.
     *
     * This should be implemented for animation support.
     *
     * @param frame
     */
    virtual void SetFrame(const pTextureRegion &frame);

    /**
     * Returns shader for drawables sorting.
     *
     * If drawable uses more than one shader this should return nullptr.
     *
     * @return shader
     */
    virtual const Shader *GetShaderForSortOrNull() const;

    /**
     * Returns texture region for drawables sorting.
     *
     * If drawable uses more than one texture (not region!) this should return nullptr.
     * If drawable uses one texture, but different texture regions, this should return one of them.
     * TextureRegion will be used for sorting with filtering and wrapping
     *
     * @return texture region
     */
    virtual const TextureRegion *GetTextureRegionForSortOrNull() const;

    virtual void Draw(SpriteBatch &sprite_batch);
};

} //namespace a2d

#endif //A2D_DRAWABLE_H
