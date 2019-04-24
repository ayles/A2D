//
// Created by selya on 25.04.19.
//

#include <a2d/renderer/framebuffer.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/renderer/texture/texture.hpp>

namespace a2d {

Framebuffer::Framebuffer() : width(0), height(0), framebuffer(0) {}

int Framebuffer::GetWidth() {
    return width;
}

int Framebuffer::GetHeight() {
    return height;
}

intrusive_ptr<Texture> Framebuffer::GetTexture() {
    return texture;
}

void Framebuffer::SetSize(int width, int height) {
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

intrusive_ptr<Framebuffer> Framebuffer::Create() {
    return new Framebuffer;
}

unsigned int &Framebuffer::GetBoundFramebuffer() {
    static unsigned int framebuffer = 0;
    return framebuffer;
}

void Framebuffer::Bind() {
    if (GetBoundFramebuffer() != framebuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        GetBoundFramebuffer() = framebuffer;
    }
}

void Framebuffer::Unbind() {
    if (GetBoundFramebuffer()) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GetBoundFramebuffer() = 0;
    }
}

}
