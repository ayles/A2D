//
// Created by selya on 29.12.2018.
//

#include <a2d/renderer/texture/texture.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/renderer/texture/texture_bind_manager.hpp>


namespace a2d {

Texture::Texture(int width, int height, const std::vector<unsigned char> &data, bool mipmaps) :
Texture(std::move(TextureBuffer(width, height, data)), mipmaps) {}

Texture::Texture(const TextureBuffer &buffer, bool mipmaps) : Texture(std::move(TextureBuffer(buffer)), mipmaps) {}

Texture::Texture(TextureBuffer &&buffer, bool mipmaps) : texture_handle(0), filtering(NEAREST), wrapping(REPEAT),
buffer(std::move(buffer)), mipmaps((
        (buffer.GetWidth() & (buffer.GetWidth() - 1)) ||
        (buffer.GetHeight() & (buffer.GetHeight() - 1))) &&
        mipmaps),
filtering_update_required(true), wrapping_update_required(true) {
    ASSERT_MAIN_THREAD
}

Texture::~Texture() {
    ASSERT_MAIN_THREAD
    Unload();
}

int Texture::GetWidth() const {
    ASSERT_MAIN_THREAD
    return buffer.GetWidth();
}

int Texture::GetHeight() const {
    ASSERT_MAIN_THREAD
    return buffer.GetHeight();
}

bool Texture::HaveMipmaps() const {
    ASSERT_MAIN_THREAD
    return mipmaps;
}

bool Texture::IsLoaded() const {
    return bool(texture_handle);
}

TextureBuffer& Texture::GetBuffer() {
    return buffer;
}

Texture::Filtering Texture::GetFiltering() const {
    return filtering;
}

Texture::Wrapping Texture::GetWrapping() const {
    return wrapping;
}

void Texture::Load() {
    ASSERT_MAIN_THREAD
    if (IsLoaded()) return;

    glGenTextures(1, &texture_handle);
    TextureBindManager::BindTexture(this);

    void *data = nullptr;
    if (!buffer.GetBuffer().empty()) data = buffer.GetBuffer().data();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if (mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Unload() {
    ASSERT_MAIN_THREAD
    if (!IsLoaded()) return;
    TextureBindManager::UnbindTexture(texture_handle);
    glDeleteTextures(1, &texture_handle);
    texture_handle = 0;
}

void Texture::FetchBuffer() {
    ASSERT_MAIN_THREAD
    if (!IsLoaded()) return;
    Bind();
    GetBuffer().Allocate();
#if RENDERER_GL
    // TODO fix for GL ES
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.GetBuffer().data());
#endif
}

void Texture::Reload() {
    Unload();
    Load();
}

void Texture::BindToGL() {
    ASSERT_MAIN_THREAD
    glBindTexture(GL_TEXTURE_2D, texture_handle);
    if (filtering_update_required) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
        filtering_update_required = false;
    }
    if (wrapping_update_required) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
        wrapping_update_required = false;
    }
}

void Texture::BindToActiveUnit() {
    ASSERT_MAIN_THREAD
    Load();
    TextureBindManager::BindTextureToCurrentUnit(this);
}

int Texture::Bind() {
    ASSERT_MAIN_THREAD
    Load();
    return TextureBindManager::BindTexture(this);
}

void Texture::SetFiltering(Texture::Filtering filtering) {
    this->filtering = filtering;
    filtering_update_required = true;
}

void Texture::SetWrapping(Texture::Wrapping wrapping) {
    this->wrapping = wrapping;
    wrapping_update_required = true;
}

pTexture Texture::Create(int width, int height, const std::vector<unsigned char> &data, bool mipmaps) {
    return new Texture(width, height, data, mipmaps);
}

pTexture Texture::Create(const TextureBuffer &buffer, bool mipmaps) {
    return new Texture(buffer, mipmaps);
}

pTexture Texture::Create(TextureBuffer &&buffer, bool mipmaps) {
    return new Texture(std::move(buffer), mipmaps);
}

unsigned int Texture::GetHandle() {
    return texture_handle;
}


} //namespace a2d