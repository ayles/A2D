//
// Created by selya on 10.02.2019.
//

#ifndef A2D_MATERIAL_H
#define A2D_MATERIAL_H

#include <a2d/core/ref_counter.hpp>
#include <a2d/renderer/shader.hpp>
#include <a2d/utils/utils.hpp>

#include <memory>
#include <map>
#include <type_traits>
#include <list>

namespace a2d {

class Texture;

class Material : public ref_counter {
    friend struct std::hash<a2d::Material>;

    class ValueBase {
    public:
        ValueBase(const std::string &name, Shader::ValueType type);
        virtual ~ValueBase() = default;

        const std::string name;
        const Shader::ValueType type;
        Shader::Uniform *uniform = nullptr;

        virtual void SetToUniform() = 0;
        virtual size_t GetHash() const = 0;
    };

    template<class T, class Dummy = void>
    class Value : public ValueBase {
    public:
        Value(const std::string &name, Shader::ValueType type);
        ~Value() override = default;

        T value;

        void SetToUniform() override;
        size_t GetHash() const override;
    };

    template<class Dummy>
    class Value<intrusive_ptr<Texture>, Dummy> : public ValueBase {
    public:
        Value(const std::string &name, Shader::ValueType type);
        ~Value() override = default;

        intrusive_ptr<Texture> value;

        void SetToUniform() override;
        size_t GetHash() const override;
    };

    std::map<std::string, std::shared_ptr<ValueBase>> values;
    intrusive_ptr<Shader> shader;
    size_t saved_hash;

    void InvalidateValue(ValueBase *value);
    void InvalidateHash();

    template<class T>
    Value<T> *GetValue(const std::string &name, Shader::ValueType type, bool create = false);

    Material(const intrusive_ptr<Shader> &shader);
    ~Material() override = default;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Material)

    void SetShader(const intrusive_ptr<Shader> &shader);
    void SetTexture(const std::string &name, const intrusive_ptr<Texture> &texture);
    void SetFloat(const std::string &name, float value);
    void SetInt(const std::string &name, int value);
    void SetVector2f(const std::string &name, const Vector2f &value);
    void SetVector3f(const std::string &name, const Vector3f &value);
    void SetVector4f(const std::string &name, const Vector4f &value);
    void SetVector2i(const std::string &name, const Vector2i &value);
    void SetVector3i(const std::string &name, const Vector3i &value);
    void SetVector4i(const std::string &name, const Vector4i &value);

    intrusive_ptr<Shader> GetShader();
    intrusive_ptr<Texture> GetTexture(const std::string &name);
    float GetFloat(const std::string &name);
    int GetInt(const std::string &name);
    Vector2f GetVector2f(const std::string &name);
    Vector3f GetVector3f(const std::string &name);
    Vector4f GetVector4f(const std::string &name);
    Vector2i GetVector2i(const std::string &name);
    Vector3i GetVector3i(const std::string &name);
    Vector4i GetVector4i(const std::string &name);

    size_t GetHash() const;

    void Bind();

    static intrusive_ptr<Material> Create(const intrusive_ptr<Shader> &shader = nullptr);
};

}

#endif //A2D_MATERIAL_H
