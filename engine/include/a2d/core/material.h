//
// Created by selya on 14.11.2018.
//

#ifndef A2D_MATERIAL_H
#define A2D_MATERIAL_H

#include <a2d/core/shader.h>
#include <a2d/core/texture.h>

namespace a2d {

class Material {
public:
    Material(pShader shader);

    void SetFloat(const std::string &name, float v);
    void SetInt(const std::string &name, int v);
    void SetVector2f(const std::string &name, const Vector2f &vec);
    void SetVector3f(const std::string &name, const Vector3f &vec);
    void SetVector4f(const std::string &name, const Vector4f &vec);
    void SetMatrix3f(const std::string &name, const Matrix3f &mat);
    void SetMatrix4f(const std::string &name, const Matrix4f &mat);
    void SetTexture(const std::string &name, const Texture &tex);

    const float *GetFloat(const std::string &name) const;
    const int *GetInt(const std::string &name) const;
    const Vector2f *GetVector2f(const std::string &name) const;
    const Vector3f *GetVector3f(const std::string &name) const;
    const Vector4f *GetVector4f(const std::string &name) const;
    const Matrix3f *GetMatrix3f(const std::string &name) const;
    const Matrix4f *GetMatrix4f(const std::string &name) const;
    const Texture *GetTexture(const std::string &name) const;

    void Bind();
    Material Clone();

    ~Material();
private:
    struct Value {
        Shader::Uniform uniform;
        void *value_ptr;
    };

    pShader shader;
    std::map<std::string, Value> values;

    static Material *bound_material;
};

} //namespace a2d

#endif //A2D_MATERIAL_H
