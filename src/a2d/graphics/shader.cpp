//
// Created by selya on 17.11.2018.
//

#include <a2d/graphics/shader.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/graphics/gl.hpp>
#include <a2d/core/log.hpp>

#include <iostream>

namespace a2d {

GLuint Shader::bound_shader_id = 0;

static GLuint CompileShader(const std::string &shader_text, const GLenum &shader_type) {
    ASSERT_MAIN_THREAD
    GLuint shader_id = glCreateShader(shader_type);

    const char *strings[] {
            shader_text.c_str()
    };

    glShaderSource(shader_id, sizeof(strings) / sizeof(const char *), strings, nullptr);

    glCompileShader(shader_id);

    GLint is_compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);

    if (is_compiled == GL_FALSE) {
        GLint max_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

        if (max_length > 0) {
            std::vector<GLchar> info_log(max_length);
            glGetShaderInfoLog(shader_id, max_length, &max_length, &info_log[0]);

            Logger::Error("{} shader: \n{}",
                                            shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                                            std::string(&*info_log.begin()));
        }
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

static GLuint CompileProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
    ASSERT_MAIN_THREAD
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint is_linked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, &is_linked);

    if (is_linked == GL_FALSE) {
        GLint max_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length);

        if (max_length > 0) {
            std::vector<GLchar> info_log(max_length);
            glGetProgramInfoLog(program_id, max_length, &max_length, &info_log[0]);

            glDeleteShader(vertex_shader_id);
            glDeleteShader(fragment_shader_id);

            glDeleteProgram(program_id);

            Logger::Error("Error linking shader:\n{}", std::string(&*info_log.begin()));
        }

        return 0;
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

Shader::Shader(const std::string &vertex_shader_text, const std::string &fragment_shader_text) :
shader_id(CompileProgram(
        CompileShader(vertex_shader_text, GL_VERTEX_SHADER),
        CompileShader(fragment_shader_text, GL_FRAGMENT_SHADER)
)) {
    ASSERT_MAIN_THREAD

    GLint max_uniform_name_length = 0;
    glGetProgramiv(shader_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_name_length);
    GLint max_attribute_name_length = 0;
    glGetProgramiv(shader_id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attribute_name_length);

    std::vector<GLchar> name_data(std::max(max_uniform_name_length, max_attribute_name_length));

    GLint uniforms_count;
    glGetProgramiv(shader_id, GL_ACTIVE_UNIFORMS, &uniforms_count);
    for (GLuint i = 0; (int)i < uniforms_count; i++) {
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(shader_id, i, name_data.size(), &length, &size, &type, &name_data[0]);
        ValueType vt;
        switch (type) {
            case GL_FLOAT: vt = ValueType::FLOAT; break;
            case GL_INT: vt = ValueType::INT; break;
            case GL_FLOAT_MAT3: vt = ValueType::MATRIX_3_FLOAT; break;
            case GL_FLOAT_MAT4: vt = ValueType::MATRIX_4_FLOAT; break;
            case GL_FLOAT_VEC2: vt = ValueType::VECTOR_2_FLOAT; break;
            case GL_FLOAT_VEC3: vt = ValueType::VECTOR_3_FLOAT; break;
            case GL_FLOAT_VEC4: vt = ValueType::VECTOR_4_FLOAT; break;
            default: vt = UNSUPPORTED; break;
        }
        unsigned int location = (unsigned int)glGetUniformLocation(shader_id, &name_data[0]);
        uniforms.insert(std::make_pair(&name_data[0], Uniform { &name_data[0], vt, location }));
    }

    GLint attributes_count;
    glGetProgramiv(shader_id, GL_ACTIVE_ATTRIBUTES, &attributes_count);
    for (GLuint i = 0; (int)i < attributes_count; i++) {
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveAttrib(shader_id, i, name_data.size(), &length, &size, &type, &name_data[0]);
        ValueType vt;
        switch (type) {
            case GL_FLOAT: vt = ValueType::FLOAT; break;
            case GL_INT: vt = ValueType::INT; break;
            case GL_FLOAT_MAT3: vt = ValueType::MATRIX_3_FLOAT; break;
            case GL_FLOAT_MAT4: vt = ValueType::MATRIX_4_FLOAT; break;
            case GL_FLOAT_VEC2: vt = ValueType::VECTOR_2_FLOAT; break;
            case GL_FLOAT_VEC3: vt = ValueType::VECTOR_3_FLOAT; break;
            case GL_FLOAT_VEC4: vt = ValueType::VECTOR_4_FLOAT; break;
            default: vt = UNSUPPORTED; break;
        }
        unsigned int location = (unsigned int)glGetAttribLocation(shader_id, &name_data[0]);
        attributes.insert(std::make_pair(&name_data[0], Attribute { &name_data[0], vt, location }));
    }
}

bool Shader::Bind() {
    ASSERT_MAIN_THREAD
    if (bound_shader_id == shader_id)
        return false;

    glUseProgram(shader_id);
    bound_shader_id = shader_id;
    return true;
}

void Shader::SetUniform(const std::string &name, float v) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::FLOAT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, int v) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::INT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, unsigned int v) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::TEXTURE_UNIT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, const Vector2f &vec) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_2_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Vector3f &vec) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_3_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Vector4f &vec) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_4_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Matrix3f &mat) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::MATRIX_3_FLOAT) return;
    uniform->Set(mat);
}

void Shader::SetUniform(const std::string &name, const Matrix4f &mat) {
    ASSERT_MAIN_THREAD
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::MATRIX_4_FLOAT) return;
    uniform->Set(mat);
}

const std::map<std::string, Shader::Uniform> &Shader::GetUniforms() const {
    ASSERT_MAIN_THREAD
    return uniforms;
}

Shader::Uniform *Shader::GetUniform(const std::string &name) const {
    ASSERT_MAIN_THREAD
    auto u = uniforms.find(name);
    if (u != uniforms.end()) {
        return const_cast<Uniform *>(&u->second);
    }
    return nullptr;
}

const std::map<std::string, Shader::Attribute> &Shader::GetAttributes() const {
    ASSERT_MAIN_THREAD
    return attributes;
}

Shader::Attribute *Shader::GetAttribute(const std::string &name) const {
    ASSERT_MAIN_THREAD
    auto u = attributes.find(name);
    if (u != attributes.end()) {
        return const_cast<Attribute *>(&u->second);
    }
    return nullptr;
}

Shader::~Shader() {
    ASSERT_MAIN_THREAD
    glDeleteProgram(shader_id);
}

void Shader::Uniform::Set(float v) {
    ASSERT_MAIN_THREAD
    glUniform1f(location, v);
}

void Shader::Uniform::Set(int v) {
    ASSERT_MAIN_THREAD
    glUniform1i(location, v);
}

void Shader::Uniform::Set(unsigned int v) {
    ASSERT_MAIN_THREAD
#ifdef RENDERER_GL
    glUniform1ui(location, v);
#elif RENDERER_GL_ES
    glUniform1i(location, v);
#endif
}

void Shader::Uniform::Set(const Vector2f &vec) {
    ASSERT_MAIN_THREAD
    glUniform2f(location, vec.x, vec.y);
}

void Shader::Uniform::Set(const Vector3f &vec) {
    ASSERT_MAIN_THREAD
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::Uniform::Set(const Vector4f &vec) {
    ASSERT_MAIN_THREAD
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::Uniform::Set(const Matrix3f &mat) {
    ASSERT_MAIN_THREAD
    // TODO
    //glUniformMatrix3fv(location, 1, GL_FALSE, mat[0]);
}

void Shader::Uniform::Set(const Matrix4f &mat) {
    ASSERT_MAIN_THREAD
    glUniformMatrix4fv(location, 1, GL_FALSE, mat[0]);
}

void Shader::Unbind() {
    ASSERT_MAIN_THREAD
    if (bound_shader_id == 0) return;

    glUseProgram(0);
    bound_shader_id = 0;
}

pShader Shader::Create(const std::string &vertex_shader_text, const std::string &fragment_shader_text) {
    ASSERT_MAIN_THREAD
    return new Shader(vertex_shader_text, fragment_shader_text);
}

} //namespace a2d