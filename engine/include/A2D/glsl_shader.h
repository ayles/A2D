//
// Created by selya on 10.11.2018.
//

#ifndef A2D_NATIVE_SHADER_H
#define A2D_NATIVE_SHADER_H

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#else
#include <GL/glew.h>
#endif

#include <string>
#include <map>

namespace a2d {

class GLSLShader {
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

    template<class T>
    bool SetUniformValue(const std::string &name, const T &data);

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
