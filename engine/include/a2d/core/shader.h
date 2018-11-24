#include <utility>

//
// Created by selya on 14.11.2018.
//

#ifndef A2D_SHADER_H
#define A2D_SHADER_H

#include <a2d/core/macro.h>
#include <a2d/math.h>
#include <a2d/core/texture.h>
#include <a2d/core/ref_counter.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <map>
#include <string>

namespace a2d {

DECLARE_SMART_POINTER(Shader)

class Shader : public ref_counter {
    friend class Renderer;
    friend class SpriteBatch;

private:
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

#if defined(RENDERER_GL) || defined(RENDERER_GL_ES)
        const GLuint location;
#endif
        void Set(float v);
        void Set(int v);
        void Set(unsigned int v);
        void Set(const Vector2f &vec);
        void Set(const Vector3f &vec);
        void Set(const Vector4f &vec);
        void Set(const Matrix3f &mat);
        void Set(const Matrix4f &mat);
    };

public:
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

    static pShader GetShader(const std::string &name);

private:
    Shader(const std::string &vertex_shader_text, const std::string &fragment_shader_text);

    virtual bool Bind();

    virtual ~Shader();

#if defined(RENDERER_GL) || defined(RENDERER_GL_ES)
    const GLuint shader_id;
#endif
    std::map<std::string, Uniform> uniforms;


    static std::map<std::string, pShader> shaders;
    static GLuint bound_shader_id;

    static GLuint CompileShader(const std::string &shader_text, const GLenum &shader_type);
    static GLuint CompileProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
};

} //namespace a2d

#endif //A2D_SHADER_H
