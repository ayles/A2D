//
// Created by selya on 10.02.2019.
//

#ifndef A2D_FRAMEBUFFER_H
#define A2D_FRAMEBUFFER_H

#include <a2d/core/ref_counter.hpp>
#include <a2d/core/intrusive_ptr.hpp>

namespace a2d {

class Texture;

class Framebuffer : ref_counter {
    friend class Camera;

    int width;
    int height;
    intrusive_ptr<Texture> texture;
    unsigned int framebuffer;

    Framebuffer();

public:
    int GetWidth();
    int GetHeight();
    intrusive_ptr<Texture> GetTexture();
    void SetSize(int width, int height);

    static intrusive_ptr<Framebuffer> Create();

private:
    static unsigned int &GetBoundFramebuffer();

    void Bind();

    static void Unbind();
};

}

#endif //A2D_FRAMEBUFFER_H
