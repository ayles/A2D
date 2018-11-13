//
// Created by selya on 10.11.2018.
//

#ifndef A2D_NATIVE_SHADER_H
#define A2D_NATIVE_SHADER_H

#include <a2d/math.h>
#include <a2d/core/ref_counter.h>

#ifdef TARGET_ANDROID
#include <GLES2/gl2.h>
#elif TARGET_DESKTOP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <string>
#include <map>

namespace a2d {

class GLSLShader : public ref_counter {
public:
    class Uniform {
    public:
        const std::string name;
        const GLenum type;
        const GLuint location;

        Uniform(std::string name, GLenum type, GLuint index) : name(std::move(name)), type(type), location(index) {}
    };

    friend class NativeRenderer;

    const std::string name;
    const GLuint shader_id;

    void Bind() const;

    const Uniform *GetUniform(const std::string &name);
    const std::map<std::string, Uniform> &GetUniforms();

    bool SetUniformValue(const std::string &name, float v);
    bool SetUniformValue(const std::string &name, double v);
    bool SetUniformValue(const std::string &name, signed char v);
    bool SetUniformValue(const std::string &name, unsigned char v);
    bool SetUniformValue(const std::string &name, short v);
    bool SetUniformValue(const std::string &name, unsigned short v);
    bool SetUniformValue(const std::string &name, int v);
    bool SetUniformValue(const std::string &name, unsigned int v);
    bool SetUniformValue(const std::string &name, long v);
    bool SetUniformValue(const std::string &name, unsigned long v);
    template<class T>
    bool SetUniformValue(const std::string &name, const Matrix<3, 3, T> &mat);
    template<class T>
    bool SetUniformValue(const std::string &name, const Matrix<4, 4, T> &mat);
    template<class T>
    bool SetUniformValue(const std::string &name, const Vector<2, T> &vec);
    template<class T>
    bool SetUniformValue(const std::string &name, const Vector<3, T> &vec);
    template<class T>
    bool SetUniformValue(const std::string &name, const Vector<4, T> &vec);

    static void Unbind();

private:
    GLSLShader(const std::string &name, const std::string &vertex_shader_text, const std::string &fragment_shader_text);
    ~GLSLShader();

    std::map<std::string, Uniform> uniforms;

    static GLuint bound_shader_id;

    static GLuint CompileShader(const std::string &shader_text, const GLenum &shader_type);
    static GLuint CompileProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
};

} // namespace a2d

#endif //A2D_NATIVE_SHADER_H
