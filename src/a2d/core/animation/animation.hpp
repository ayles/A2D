//
// Created by selya on 05.03.2019.
//

#ifndef A2D_ANIMATION_HPP
#define A2D_ANIMATION_HPP

#include <vector>
#include <functional>

namespace a2d {

class Object2D;

class Animation {
    template<class Animation>
    friend class Animator;

    Object2D *object_2d;

public:
    intrusive_ptr<Object2D> GetObject2D();

    virtual void Start() {}
    virtual void End() {}
    virtual void Step(float time) = 0;
};

class Animations {
public:
    class ClearColorTo : public Animation {
        Vector4f color;
        Vector4f color_from;

    public:
        ClearColorTo(const Vector4f &color);

        void Start() override;
        void Step(float time) override;
    };

    class ColorTo : public Animation {
        Vector4f color;
        Vector4f color_from;

    public:
        ColorTo(const Vector4f &color);

        void Start() override;
        void Step(float time) override;
    };

    class Frames : public Animation {
        std::vector<pTextureRegion> frames;

    public:
        Frames(const std::vector<pTextureRegion> &frames);

        void Start() override;
        void Step(float time) override;
    };
};

}


#endif //A2D_ANIMATION_HPP
