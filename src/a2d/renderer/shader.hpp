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

class Shader final : public ref_counter {
public:
    enum ValueType {
        FLOAT = 1,
        INT,
        UNSIGNED_INT,
        MATRIX_3_FLOAT,
        MATRIX_4_FLOAT,
        VECTOR_2_FLOAT,
        VECTOR_3_FLOAT,
        VECTOR_4_FLOAT,
        VECTOR_2_INT,
        VECTOR_3_INT,
        VECTOR_4_INT,
        VECTOR_2_UNSIGNED_INT,
        VECTOR_3_UNSIGNED_INT,
        VECTOR_4_UNSIGNED_INT,
        TEXTURE_UNIT,
        UNSUPPORTED
    };

    struct Attribute {
        const std::string name;
        const ValueType type;
        const unsigned int location;
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
        void Set(const Vector2i &vec);
        void Set(const Vector3i &vec);
        void Set(const Vector4i &vec);
        void Set(const Vector2ui &vec);
        void Set(const Vector3ui &vec);
        void Set(const Vector4ui &vec);
        void Set(const Matrix3f &mat);
        void Set(const Matrix4f &mat);
    };

    DELETE_DEFAULT_CONSTRUCTORS_AND_OPERATORS(Shader)

    void SetUniform(const std::string &name, float v);
    void SetUniform(const std::string &name, int v);
    void SetUniform(const std::string &name, unsigned int v);
    void SetUniform(const std::string &name, const Vector2f &vec);
    void SetUniform(const std::string &name, const Vector3f &vec);
    void SetUniform(const std::string &name, const Vector4f &vec);
    void SetUniform(const std::string &name, const Vector2i &vec);
    void SetUniform(const std::string &name, const Vector3i &vec);
    void SetUniform(const std::string &name, const Vector4i &vec);
    void SetUniform(const std::string &name, const Vector2ui &vec);
    void SetUniform(const std::string &name, const Vector3ui &vec);
    void SetUniform(const std::string &name, const Vector4ui &vec);
    void SetUniform(const std::string &name, const Matrix3f &mat);
    void SetUniform(const std::string &name, const Matrix4f &mat);

    Uniform *GetUniform(const std::string &name) const;
    const std::map<std::string, Uniform> &GetUniforms() const;

    Attribute *GetAttribute(const std::string &name) const;
    const std::map<std::string, Attribute> &GetAttributes() const;

    void Bind();

    static pShader Create(const std::string &vertex_shader_text, const std::string &fragment_shader_text);
    static void Unbind();
    static void CreateDefaultShaders();

private:
    const unsigned int shader_id;
    std::map<std::string, Uniform> uniforms;
    std::map<std::string, Attribute> attributes;

    Shader(const std::string &vertex_shader_text, const std::string &fragment_shader_text);
    ~Shader() override;

    static pShader bound_shader;
};

} //namespace a2d

#endif //A2D_SHADER_H
