//
// Created by selya on 29.12.2018.
//

#include <a2d/renderer/texture.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/core/engine.hpp>


namespace a2d {

std::vector<unsigned int> Texture::unit_to_handle;
std::map<unsigned int, int> Texture::handle_to_unit;
int Texture::current_unit = 0;

Texture::Texture(int width, int height, const std::vector<unsigned char> &data, bool mipmaps) :
Texture(std::move(TextureBuffer(width, height, data)), mipmaps) {}

Texture::Texture(const TextureBuffer &buffer, bool mipmaps) : Texture(std::move(TextureBuffer(buffer)), mipmaps) {}

Texture::Texture(TextureBuffer &&buffer, bool mipmaps) : texture_handle(0), filtering(NEAREST), wrapping(REPEAT),
buffer(std::move(buffer)), mipmaps((
        (buffer.GetWidth() & (buffer.GetWidth() - 1)) ||
        (buffer.GetHeight() & (buffer.GetHeight() - 1))) &&
        mipmaps) {
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
    glBindTexture(GL_TEXTURE_2D, texture_handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

    void *data = nullptr;
    if (!buffer.GetBuffer().empty()) data = buffer.GetBuffer().data();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWidth(), GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if (mipmaps) glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Unload() {
    ASSERT_MAIN_THREAD
    if (!IsLoaded()) return;
    glDeleteTextures(1, &texture_handle);
    texture_handle = 0;
}

void Texture::FetchBuffer() {
    ASSERT_MAIN_THREAD
    if (!IsLoaded()) return;
    GetBuffer().Allocate();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.GetBuffer().data());
}

void Texture::Reload() {
    Unload();
    Load();
}

void Texture::Bind() {
    ASSERT_MAIN_THREAD
    if (!IsLoaded()) Load();
    else glBindTexture(GL_TEXTURE_2D, texture_handle);
}

void Texture::Bind(int texture_unit) {
    ASSERT_MAIN_THREAD
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    Bind();
}

int Texture::SmartBind() {
    if (IsLoaded()) {
        auto iter = handle_to_unit.find(texture_handle);
        if (iter != handle_to_unit.end()) {
            return iter->second;
        }
    }
    int unit = current_unit;
    Bind(unit);
    if (++current_unit >= unit_to_handle.size()) {
        current_unit = 0;
    }
    unsigned int handle_to_unbind = unit_to_handle[unit];
    handle_to_unit.erase(handle_to_unbind);
    handle_to_unit.emplace(texture_handle, unit);
    unit_to_handle[unit] = texture_handle;
    return unit;
}

void Texture::SetFiltering(Texture::Filtering filtering) {
    this->filtering = filtering;
    if (!IsLoaded()) return;
    glActiveTexture(GL_TEXTURE0 + SmartBind());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
}

void Texture::SetWrapping(Texture::Wrapping wrapping) {
    this->wrapping = wrapping;
    if (!IsLoaded()) return;
    glActiveTexture(GL_TEXTURE0 + SmartBind());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
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