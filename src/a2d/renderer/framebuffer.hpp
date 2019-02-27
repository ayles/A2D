//
// Created by selya on 10.02.2019.
//

#ifndef A2D_FRAMEBUFFER_H
#define A2D_FRAMEBUFFER_H

#include <a2d/renderer/texture.hpp>
#include <a2d/renderer/gl.hpp>

namespace a2d {

DECLARE_SMART_POINTER(Framebuffer)

class Framebuffer : ref_counter {
    friend class Camera;

    int width;
    int height;
    pTexture texture;
    GLuint framebuffer;

    Framebuffer() : width(0), height(0), framebuffer(0) {}

public:
    int GetWidth() {
        return width;
    }

    int GetHeight() {
        return height;
    }

    pTexture GetTexture() {
        return texture;
    }

    void SetSize(int width, int height) {
        this->width = width;
        this->height = height;
        texture = Texture::Create(width, height);
        texture->Load();
        if (framebuffer) {
            glDeleteFramebuffers(1, &framebuffer);
        }
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetHandle(), 0);
        //GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
        //glDrawBuffers(1, draw_buffers);
        glBindFramebuffer(GL_FRAMEBUFFER, GetBoundFramebuffer());

    }

    static pFramebuffer Create() {
        return new Framebuffer;
    }

private:
    static GLuint &GetBoundFramebuffer() {
        static GLuint framebuffer = 0;
        return framebuffer;
    }

    void Bind() {
        if (GetBoundFramebuffer() != framebuffer) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            GetBoundFramebuffer() = framebuffer;
        }
    }

    static void Unbind() {
        if (GetBoundFramebuffer()) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            GetBoundFramebuffer() = 0;
        }
    }
};

}

#endif //A2D_FRAMEBUFFER_H
