//
// Created by selya on 14.11.2018.
//

#ifndef A2D_SHADER_H
#define A2D_SHADER_H

#include <a2d/core/macro.hpp>
#include <a2d/math/math.hpp>
#include <a2d/core/ref_counter.hpp>

#include <map>
#include <string>

namespace a2d {

DECLARE_SMART_POINTER(Shader)

class Shader : public ref_counter {
    friend class Renderer;
    friend class SpriteBatch;

public:
    enum ValueType {
        FLOAT = 1,
        INT,
        MATRIX_3_FLOAT,
        MATRIX_4_FLOAT,
        VECTOR_2_FLOAT,
        VECTOR_3_FLOAT,
        VECTOR_4_FLOAT,
        TEXTURE_UNIT,
        UNSUPPORTED
    };

    struct Uniform {
        const std::string name;
        const ValueType type;
        const unsigned int location;

        void Set(float v);
        void Set(int v);
        void Set(unsigned int v);
        void Set(const Vector2f &vec);
        void Set(const Vector3f &vec);
        void Set(const Vector4f &vec);
        void Set(const Matrix3f &mat);
        void Set(const Matrix4f &mat);
    };

    struct Attribute {
        const std::string name;
        const ValueType type;
        const unsigned int location;
    };

private:
    const unsigned int shader_id;
    std::map<std::string, Uniform> uniforms;
    std::map<std::string, Attribute> attributes;

    static unsigned int bound_shader_id;

    static void CreateDefaultShaders();

    Shader(const std::string &vertex_shader_text, const std::string &fragment_shader_text);
    ~Shader() override;

public:
    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Shader)

    virtual void SetUniform(const std::string &name, float v);
    virtual void SetUniform(const std::string &name, int v);
    virtual void SetUniform(const std::string &name, unsigned int v);
    virtual void SetUniform(const std::string &name, const Vector2f &vec);
    virtual void SetUniform(const std::string &name, const Vector3f &vec);
    virtual void SetUniform(const std::string &name, const Vector4f &vec);
    virtual void SetUniform(const std::string &name, const Matrix3f &mat);
    virtual void SetUniform(const std::string &name, const Matrix4f &mat);

    virtual const std::map<std::string, Uniform> &GetUniforms() const;
    virtual Uniform *GetUniform(const std::string &name) const;

    virtual const std::map<std::string, Attribute> &GetAttributes() const;
    virtual Attribute *GetAttribute(const std::string &name) const;

    virtual bool Bind();

    static pShader Create(const std::string &vertex_shader_text, const std::string &fragment_shader_text);

    static void Unbind();
};

} //namespace a2d

#endif //A2D_SHADER_H
