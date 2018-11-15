//
// Created by selya on 10.11.2018.
//

#include <a2d/core/glsl_shader.h>
#include <a2d/math/vector.h>
#include <a2d/math.h>
#include <a2d/core.h>

#include <utility>
#include <vector>
#include <iostream>

GLuint a2d::GLSLShader::bound_shader_id = 0;

a2d::GLSLShader::GLSLShader(
        const std::string &name,
        const std::string &vertex_shader_text, const std::string &fragment_shader_text
) : name(name), shader_id(CompileProgram(
        CompileShader(vertex_shader_text, GL_VERTEX_SHADER),
        CompileShader(fragment_shader_text, GL_FRAGMENT_SHADER)
)) {
    GLint max_uniform_name_length = 0;
    glGetProgramiv(shader_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);

    std::vector<GLchar> name_data(max_uniform_name_length);

    GLint uniforms_count;
    glGetProgramiv(shader_id, GL_ACTIVE_UNIFORMS, &uniforms_count);
    for (GLuint i = 0; i < uniforms_count; i++) {
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(shader_id, i, name_data.size(), &length, &size, &type, &name_data[0]);
        uniforms.insert(std::make_pair(&name_data[0], Uniform(&name_data[0], type, i)));
    }
}

a2d::GLSLShader::~GLSLShader() {
    glDeleteProgram(shader_id);
}

void a2d::GLSLShader::Bind() const {
    if (bound_shader_id == shader_id)
        return;

    glUseProgram(shader_id);
    bound_shader_id = shader_id;
}

void a2d::GLSLShader::Unbind() {
    glUseProgram(0);
    bound_shader_id = 0;
}

GLuint a2d::GLSLShader::CompileShader(const std::string &shader_text, const GLenum &shader_type) {
    GLuint shader_id = glCreateShader(shader_type);

#ifdef TARGET_ANDROID
    const int num_strings = 2;
    const char *strings[num_strings] {
        "#define ES\n",
        shader_text.c_str()
    };
#elif TARGET_DESKTOP
    const int num_strings = 2;
    const char *strings[num_strings] {
        "#version 330 core\n",
        shader_text.c_str()
    };
#endif

    glShaderSource(shader_id, num_strings, strings, nullptr);

    glCompileShader(shader_id);

    GLint is_compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(shader_id, max_length, &max_length, &info_log[0]);

        a2d::Engine::GetLogger()->error("{} shader: \n{}",
                shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", std::string(&*info_log.begin()));

        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

GLuint a2d::GLSLShader::CompileProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint is_linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetProgramInfoLog(program_id, max_length, &max_length, &info_log[0]);

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        glDeleteProgram(program_id);

        a2d::Engine::GetLogger()->error("Error linking shader:\n{}", std::string(&*info_log.begin()));

        return 0;
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

const a2d::GLSLShader::Uniform *a2d::GLSLShader::GetUniform(const std::string &name) {
    auto u = uniforms.find(name);
    return (u == uniforms.end()) ? nullptr : &u->second;
}

const std::map<std::string, a2d::GLSLShader::Uniform> &a2d::GLSLShader::GetUniforms() {
    return uniforms;
}


template<>
bool a2d::GLSLShader::SetUniformValue(const std::string &name, const a2d::Vector2f &data) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_FLOAT_VEC2) return false;
    glUniform2f(uniform->location, data.x, data.y);
    return true;
}

template<>
bool a2d::GLSLShader::SetUniformValue(const std::string &name, const a2d::Vector3f &data) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_FLOAT_VEC3) return false;
    glUniform3f(uniform->location, data.x, data.y, data.z);
    return true;
}

template<>
bool a2d::GLSLShader::SetUniformValue(const std::string &name, const a2d::Vector4f &data) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_FLOAT_VEC4) return false;
    glUniform4f(uniform->location, data.x, data.y, data.z, data.w);
    return true;
}

template<>
bool a2d::GLSLShader::SetUniformValue(const std::string &name, const a2d::Matrix4f &data) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_FLOAT_MAT4) return false;
    glUniformMatrix4fv(uniform->location, 1, GL_FALSE, data[0]);
    return true;
}

bool a2d::GLSLShader::SetUniformValue(const std::string &name, float data) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_FLOAT) return false;
    glUniform1f(uniform->location, data);
    return true;
}

bool a2d::GLSLShader::SetUniformValue(const std::string &name, int v) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != GL_INT) return false;
    glUniform1i(uniform->location, v);
    return true;
}
