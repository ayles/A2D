//
// Created by selya on 24.04.19.
//

#include <a2d/renderer/material.hpp>
#include <a2d/renderer/texture/texture.hpp>

namespace a2d {

size_t Material::GetHash() const {
    return saved_hash;
}

void Material::SetShader(const intrusive_ptr <Shader> &shader) {
    this->shader = shader;
    for (auto &v : values) {
        InvalidateValue(v.second.get());
    }
    InvalidateHash();
}

intrusive_ptr <Shader> Material::GetShader() {
    return shader;
}

intrusive_ptr <Texture> Material::GetTexture(const std::string &name) {
    auto value = GetValue<intrusive_ptr<Texture>>(name, Shader::ValueType::TEXTURE_UNIT);
    return !value ? nullptr : value->value;
}

intrusive_ptr <Material> Material::Create(const intrusive_ptr <Shader> &shader) {
    return new Material(shader);
}

void Material::Bind() {
    if (!shader) return;
    shader->Bind();
    for (auto &v : values) {
        v.second->SetToUniform();
    }
}

void Material::SetVector4i(const std::string &name, const Vector4i &value) {
    GetValue<Vector4i>(name, Shader::ValueType::VECTOR_4_INT, true)
            ->value = value;
    InvalidateHash();
}

Vector4f Material::GetVector4i(const std::string &name) {
    auto value = GetValue<Vector4i>(name, Shader::ValueType::VECTOR_4_INT);
    return !value ? Vector4i() : value->value;
}

void Material::SetVector3i(const std::string &name, const Vector3i &value) {
    GetValue<Vector3i>(name, Shader::ValueType::VECTOR_3_INT, true)
            ->value = value;
    InvalidateHash();
}

Vector3f Material::GetVector3i(const std::string &name) {
    auto value = GetValue<Vector3i>(name, Shader::ValueType::VECTOR_3_INT);
    return !value ? Vector3i() : value->value;
}

void Material::SetVector2i(const std::string &name, const Vector2i &value) {
    GetValue<Vector2i>(name, Shader::ValueType::VECTOR_2_INT, true)
            ->value = value;
    InvalidateHash();
}

Vector2f Material::GetVector2i(const std::string &name) {
    auto value = GetValue<Vector2i>(name, Shader::ValueType::VECTOR_2_INT);
    return !value ? Vector2i() : value->value;
}

void Material::SetVector4f(const std::string &name, const Vector4f &value) {
    GetValue<Vector4f>(name, Shader::ValueType::VECTOR_4_FLOAT, true)
            ->value = value;
    InvalidateHash();
}

Vector4f Material::GetVector4f(const std::string &name) {
    auto value = GetValue<Vector4f>(name, Shader::ValueType::VECTOR_4_FLOAT);
    return !value ? Vector4f() : value->value;
}

void Material::SetVector3f(const std::string &name, const Vector3f &value) {
    GetValue<Vector3f>(name, Shader::ValueType::VECTOR_3_FLOAT, true)
            ->value = value;
    InvalidateHash();
}

Vector3f Material::GetVector3f(const std::string &name) {
    auto value = GetValue<Vector3f>(name, Shader::ValueType::VECTOR_3_FLOAT);
    return !value ? Vector3f() : value->value;
}

void Material::SetVector2f(const std::string &name, const Vector2f &value) {
    GetValue<Vector2f>(name, Shader::ValueType::VECTOR_2_FLOAT, true)
            ->value = value;
    InvalidateHash();
}

Vector2f Material::GetVector2f(const std::string &name) {
    auto value = GetValue<Vector2f>(name, Shader::ValueType::VECTOR_2_FLOAT);
    return !value ? Vector2f() : value->value;
}

void Material::SetInt(const std::string &name, int value) {
    GetValue<int>(name, Shader::ValueType::INT, true)->value = value;
    InvalidateHash();
}

int Material::GetInt(const std::string &name) {
    auto value = GetValue<int>(name, Shader::ValueType::INT);
    return !value ? 0 : value->value;
}

void Material::SetFloat(const std::string &name, float value) {
    GetValue<float>(name, Shader::ValueType::FLOAT, true)->value = value;
    InvalidateHash();
}

float Material::GetFloat(const std::string &name) {
    auto value = GetValue<float>(name, Shader::ValueType::FLOAT);
    return !value ? 0.0f : value->value;
}

void Material::SetTexture(const std::string &name, const intrusive_ptr <Texture> &texture) {
    GetValue<intrusive_ptr<Texture>>(name, Shader::ValueType::TEXTURE_UNIT, true)
            ->value = texture;
    InvalidateHash();
}

void Material::InvalidateValue(Material::ValueBase *value) {
    if (!shader) return;
    auto uniform = shader->GetUniform(value->name);
    if (uniform && value->type == uniform->type) {
        value->uniform = uniform;
    } else {
        value->uniform = nullptr;
    }
}

void Material::InvalidateHash() {
    std::hash<void *> v_hasher;
    saved_hash = v_hasher((void *)shader.get());
    for (auto &v : values) {
        utils::hash_combine(saved_hash, v.second->GetHash());
    }
}

template<class T>
Material::Value<T> *Material::GetValue(const std::string &name, Shader::ValueType type, bool create) {
    auto iter = values.find(name);
    if (iter == values.end()) {
        if (!create) return nullptr;
        auto v = new Value<T>(name, type);
        InvalidateValue(v);
        values.emplace(name, std::shared_ptr<ValueBase>(v));
        return v;
    }
    auto v_b = iter->second.get();
    if (v_b->type != type) return nullptr;
    return (Value<T> *)v_b;
}

Material::Material(const intrusive_ptr <Shader> &shader) {
    SetShader(shader);
}

Material::ValueBase::ValueBase(const std::string &name, Shader::ValueType type) :
        name(name), type(type) {}

template<class T, class Dummy>
size_t Material::Value<T, Dummy>::GetHash() const {
    std::hash<std::string> s_hasher;
    size_t hash_ = s_hasher(name);
    utils::hash_combine(hash_, value);
    return hash_;
}

template<class T, class Dummy>
void Material::Value<T, Dummy>::SetToUniform() {
    if (uniform) uniform->Set(value);
}

template<class T, class Dummy>
Material::Value<T, Dummy>::Value(const std::string &name, Shader::ValueType type) :
        ValueBase(name, type) {}

template<class Dummy>
Material::Value<intrusive_ptr<Texture>, Dummy>::Value(const std::string &name, Shader::ValueType type) :
        ValueBase(name, type) {}

template<class Dummy>
void Material::Value<intrusive_ptr<Texture>, Dummy>::SetToUniform() {
    if (uniform) uniform->Set(value->Bind());
}

template<class Dummy>
size_t Material::Value<intrusive_ptr<Texture>, Dummy>::GetHash() const {
    std::hash<std::string> s_hasher;
    size_t hash_ = s_hasher(name);
    utils::hash_combine(hash_, (void *)value.get());
    return hash_;
}

}


