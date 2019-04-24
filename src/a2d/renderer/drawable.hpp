//
// Created by selya on 24.11.2018.
//

#ifndef A2D_DRAWABLE_HPP
#define A2D_DRAWABLE_HPP

#include <a2d/core/component.hpp>
#include <a2d/core/macro.hpp>
#include <a2d/math/vector.hpp>

namespace a2d {

class TextureRegion;
class Material;
class SpriteBatch;

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

    virtual const intrusive_ptr<TextureRegion> GetTextureRegion() const final;
    virtual const intrusive_ptr<Material> GetMaterial() const final;

    virtual void SetWidth(float width);
    virtual void SetHeight(float height);
    virtual void SetSize(float x, float y);
    virtual void SetSize(const Vector2f &size);
    virtual void SetOrigin(float x, float y);
    virtual void SetOrigin(const Vector2f &origin);
    virtual void SetTextureRegion(const intrusive_ptr<TextureRegion> &texture_region);
    virtual void SetMaterial(const intrusive_ptr<Material> &material);


    /**
     * Compares drawables for minimizing shader/texture switches.
     */
    virtual bool IsLess(const Drawable &other) const final;

protected:
    Vector2f size;
    Vector2f origin;
    intrusive_ptr<TextureRegion> texture_region;
    intrusive_ptr<Material> material;

    virtual void Draw(SpriteBatch &sprite_batch, const Vector4f &color);

    void Initialize() override;
    void OnDestroy() override;
};

} //namespace a2d

#endif //A2D_DRAWABLE_HPP
