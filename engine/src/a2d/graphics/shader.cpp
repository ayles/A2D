//
// Created by selya on 17.11.2018.
//

#include <a2d/graphics/shader.h>
#include <a2d/core/engine.h>
#include <a2d/filesystem/filesystem.h>
#include <a2d/core/gl.h>

#include <vector>
#include <iostream>

namespace a2d {

GLuint Shader::bound_shader_id = 0;
std::map<std::string, pShader> Shader::shaders;

static GLuint CompileShader(const std::string &shader_text, const GLenum &shader_type) {
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

            a2d::Engine::GetLogger()->error("{} shader: \n{}",
                                            shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                                            std::string(&*info_log.begin()));
        }
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

static GLuint CompileProgram(GLuint vertex_shader_id, GLuint fragment_shader_id) {
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

            a2d::Engine::GetLogger()->error("Error linking shader:\n{}", std::string(&*info_log.begin()));
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
        uniforms.insert(std::make_pair(&name_data[0], Uniform { &name_data[0], vt, i }));
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
        attributes.insert(std::make_pair(&name_data[0], Attribute { &name_data[0], vt, i }));
    }
}

bool Shader::Bind() {
    if (bound_shader_id == shader_id)
        return false;

    glUseProgram(shader_id);
    bound_shader_id = shader_id;
    return true;
}

void Shader::SetUniform(const std::string &name, float v) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::FLOAT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, int v) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::INT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, unsigned int v) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::TEXTURE_UNIT) return;
    uniform->Set(v);
}

void Shader::SetUniform(const std::string &name, const Vector2f &vec) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_2_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Vector3f &vec) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_3_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Vector4f &vec) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::VECTOR_4_FLOAT) return;
    uniform->Set(vec);
}

void Shader::SetUniform(const std::string &name, const Matrix3f &mat) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::MATRIX_3_FLOAT) return;
    uniform->Set(mat);
}

void Shader::SetUniform(const std::string &name, const Matrix4f &mat) {
    auto uniform = GetUniform(name);
    if (bound_shader_id != shader_id || !uniform || uniform->type != ValueType::MATRIX_4_FLOAT) return;
    uniform->Set(mat);
}

const std::map<std::string, Shader::Uniform> &Shader::GetUniforms() const {
    return uniforms;
}

Shader::Uniform *Shader::GetUniform(const std::string &name) const {
    auto u = uniforms.find(name);
    if (u != uniforms.end()) {
        return const_cast<Uniform *>(&u->second);
    }
    return nullptr;
}

const std::map<std::string, Shader::Attribute> &Shader::GetAttributes() const {
    return attributes;
}

Shader::Attribute *Shader::GetAttribute(const std::string &name) const {
    auto u = attributes.find(name);
    if (u != attributes.end()) {
        return const_cast<Attribute *>(&u->second);
    }
    return nullptr;
}

Shader::~Shader() {
    glDeleteProgram(shader_id);
}

void Shader::Uniform::Set(float v) {
    glUniform1f(location, v);
}

void Shader::Uniform::Set(int v) {
    glUniform1i(location, v);
}

void Shader::Uniform::Set(unsigned int v) {
#ifdef RENDERER_GL
    glUniform1ui(location, v);
#elif RENDERER_GL_ES
    glUniform1i(location, v);
#endif
}

void Shader::Uniform::Set(const Vector2f &vec) {
    glUniform2f(location, vec.x, vec.y);
}

void Shader::Uniform::Set(const Vector3f &vec) {
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::Uniform::Set(const Vector4f &vec) {
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::Uniform::Set(const Matrix3f &mat) {
    // TODO
    //glUniformMatrix3fv(location, 1, GL_FALSE, mat[0]);
}

void Shader::Uniform::Set(const Matrix4f &mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, mat[0]);
}

pShader Shader::GetShader(const std::string &name) {
    auto s = shaders.find(name);
    if (s != shaders.end()) {
        return s->second;
    }

    pShader shader = new Shader(
        FileSystem::LoadText("shaders/" + name + "/vertex.glsl"),
        FileSystem::LoadText("shaders/" + name + "/fragment.glsl")
    );

    shaders[name] = shader;
    return shader;
}

void Shader::Unbind() {
    if (bound_shader_id == 0) return;

    glUseProgram(0);
    bound_shader_id = 0;
}

} //namespace a2d