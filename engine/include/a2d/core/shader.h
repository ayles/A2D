#include <utility>

//
// Created by selya on 14.11.2018.
//

#ifndef A2D_SHADER_H
#define A2D_SHADER_H

#include <a2d/core/macro.h>
#include <a2d/math.h>
#include <a2d/core/texture.h>

#include <map>
#include <string>

namespace a2d {

DECLARE_SMART_POINTER(Shader)

class Shader {
public:
    enum ValueType {
        FLOAT = 1,
        INT,
        MATRIX_3_FLOAT,
        MATRIX_4_FLOAT,
        VECTOR_2_FLOAT,
        VECTOR_3_FLOAT,
        VECTOR_4_FLOAT,
        TEXTURE,
        UNSUPPORTED
    };

    class Uniform {
    public:
        const std::string name;
        const ValueType type;

        Uniform(std::string name, ValueType type) : name(std::move(name)), type(type) {}
    };

    void Bind();

    void SetFloat(const std::string &name, float v);
    void SetInt(const std::string &name, int v);
    void SetVector2f(const std::string &name, const Vector2f &vec);
    void SetVector3f(const std::string &name, const Vector3f &vec);
    void SetVector4f(const std::string &name, const Vector4f &vec);
    void SetMatrix3f(const std::string &name, const Matrix3f &mat);
    void SetMatrix4f(const std::string &name, const Matrix4f &mat);
    void SetTexture(const std::string &name, const Texture &tex);

    const std::map<std::string, Uniform> &GetUniforms();
    const Uniform *GetUniform(const std::string name);
};

} //namespace a2d

#endif //A2D_SHADER_H
