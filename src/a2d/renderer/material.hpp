//
// Created by selya on 10.02.2019.
//

#ifndef A2D_MATERIAL_H
#define A2D_MATERIAL_H

#include <a2d/core/ref_counter.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/renderer/texture/texture.hpp>
#include <a2d/utils/utils.hpp>

#include <memory>
#include <map>
#include <type_traits>
#include <list>

namespace a2d {

DECLARE_SMART_POINTER(Material)

class Material : public ref_counter {
    friend struct std::hash<a2d::Material>;

    class ValueBase {
    public:
        ValueBase(const std::string &name, Shader::ValueType type) :
                name(name), type(type) {}

        const std::string name;
        const Shader::ValueType type;
        Shader::Uniform *uniform = nullptr;

        virtual void SetToUniform() = 0;
        virtual size_t GetHash() const = 0;

        virtual ~ValueBase() = default;
    };

    template<class T, class Dummy = void>
    class Value : public ValueBase {
    public:
        Value(const std::string &name, Shader::ValueType type) :
                ValueBase(name, type) {}

        T value;

        void SetToUniform() override {
            if (uniform) uniform->Set(value);
        }

        size_t GetHash() const override {
            std::hash<std::string> s_hasher;
            size_t hash_ = s_hasher(name);
            a2d::utils::hash_combine(hash_, value);
            return hash_;
        }

        ~Value() override = default;
    };

    template<class Dummy>
    class Value<pTexture, Dummy> : public ValueBase {
    public:
        Value(const std::string &name, Shader::ValueType type) :
                ValueBase(name, type) {}

        pTexture value;

        void SetToUniform() override {
            if (uniform) uniform->Set(value->Bind());
        }

        size_t GetHash() const override {
            std::hash<std::string> s_hasher;
            size_t hash_ = s_hasher(name);
            a2d::utils::hash_combine(hash_, (void *)value.get());
            return hash_;
        }

        ~Value() override = default;
    };

    std::map<std::string, std::shared_ptr<ValueBase>> values;
    pShader shader;
    size_t saved_hash;

    void InvalidateValue(ValueBase *value) {
        if (!shader) return;
        auto uniform = shader->GetUniform(value->name);
        if (uniform && value->type == uniform->type) {
            value->uniform = uniform;
        } else {
            value->uniform = nullptr;
        }
    }

    void InvalidateHash() {
        std::hash<void *> v_hasher;
        saved_hash = v_hasher((void *)shader.get());
        for (auto &v : values) {
            a2d::utils::hash_combine(saved_hash, v.second->GetHash());
        }
    }

    template<class T>
    Value<T> *GetValue(const std::string &name, Shader::ValueType type, bool create = false) {
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

    Material(const pShader &shader) {
        SetShader(shader);
    }

    ~Material() override = default;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Material)

    size_t GetHash() const {
        return saved_hash;
    }

    void SetShader(const pShader &shader) {
        this->shader = shader;
        for (auto &v : values) {
            InvalidateValue(v.second.get());
        }
        InvalidateHash();
    }

    pShader GetShader() {
        return shader;
    }

    pTexture GetTexture(const std::string &name) {
        auto value = GetValue<pTexture>(name, Shader::ValueType::TEXTURE_UNIT);
        return !value ? nullptr : value->value;
    }

    void SetTexture(const std::string &name, const pTexture &texture) {
        GetValue<pTexture>(name, Shader::ValueType::TEXTURE_UNIT, true)
                ->value = texture;
        InvalidateHash();
    }

    float GetFloat(const std::string &name) {
        auto value = GetValue<float>(name, Shader::ValueType::FLOAT);
        return !value ? 0.0f : value->value;
    }

    void SetFloat(const std::string &name, float value) {
        GetValue<float>(name, Shader::ValueType::FLOAT, true)->value = value;
        InvalidateHash();
    }

    int GetInt(const std::string &name) {
        auto value = GetValue<int>(name, Shader::ValueType::INT);
        return !value ? 0 : value->value;
    }

    void SetInt(const std::string &name, int value) {
        GetValue<int>(name, Shader::ValueType::INT, true)->value = value;
        InvalidateHash();
    }

    Vector2f GetVector2f(const std::string &name) {
        auto value = GetValue<Vector2f>(name, Shader::ValueType::VECTOR_2_FLOAT);
        return !value ? Vector2f() : value->value;
    }

    void SetVector2f(const std::string &name, const Vector2f &value) {
        GetValue<Vector2f>(name, Shader::ValueType::VECTOR_2_FLOAT, true)
                ->value = value;
        InvalidateHash();
    }

    Vector3f GetVector3f(const std::string &name) {
        auto value = GetValue<Vector3f>(name, Shader::ValueType::VECTOR_3_FLOAT);
        return !value ? Vector3f() : value->value;
    }

    void SetVector3f(const std::string &name, const Vector3f &value) {
        GetValue<Vector3f>(name, Shader::ValueType::VECTOR_3_FLOAT, true)
                ->value = value;
        InvalidateHash();
    }

    Vector4f GetVector4f(const std::string &name) {
        auto value = GetValue<Vector4f>(name, Shader::ValueType::VECTOR_4_FLOAT);
        return !value ? Vector4f() : value->value;
    }

    void SetVector4f(const std::string &name, const Vector4f &value) {
        GetValue<Vector4f>(name, Shader::ValueType::VECTOR_4_FLOAT, true)
                ->value = value;
        InvalidateHash();
    }

    Vector2f GetVector2i(const std::string &name) {
        auto value = GetValue<Vector2i>(name, Shader::ValueType::VECTOR_2_INT);
        return !value ? Vector2i() : value->value;
    }

    void SetVector2i(const std::string &name, const Vector2i &value) {
        GetValue<Vector2i>(name, Shader::ValueType::VECTOR_2_INT, true)
                ->value = value;
        InvalidateHash();
    }

    Vector3f GetVector3i(const std::string &name) {
        auto value = GetValue<Vector3i>(name, Shader::ValueType::VECTOR_3_INT);
        return !value ? Vector3i() : value->value;
    }

    void SetVector3i(const std::string &name, const Vector3i &value) {
        GetValue<Vector3i>(name, Shader::ValueType::VECTOR_3_INT, true)
                ->value = value;
        InvalidateHash();
    }

    Vector4f GetVector4i(const std::string &name) {
        auto value = GetValue<Vector4i>(name, Shader::ValueType::VECTOR_4_INT);
        return !value ? Vector4i() : value->value;
    }

    void SetVector4i(const std::string &name, const Vector4i &value) {
        GetValue<Vector4i>(name, Shader::ValueType::VECTOR_4_INT, true)
                ->value = value;
        InvalidateHash();
    }

    void Bind() {
        if (!shader) return;
        shader->Bind();
        for (auto &v : values) {
            v.second->SetToUniform();
        }
    }

    static pMaterial Create(const pShader &shader = nullptr) {
        return new Material(shader);
    }
};

}

#endif //A2D_MATERIAL_H
