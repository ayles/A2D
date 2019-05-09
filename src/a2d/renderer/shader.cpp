//
// Created by selya on 17.11.2018.
//

#include <a2d/renderer/shader.hpp>
#include <a2d/renderer/gl.hpp>
#include <a2d/core/engine.hpp>
#include <a2d/core/log.hpp>

#include <iostream>


namespace a2d {

intrusive_ptr<Shader> Shader::bound_shader;

static GLuint CompileShader(const std::string &shader_text, const GLenum &shader_type) {
    ASSERT_MAIN_THREAD
    GLuint shader_id = glCreateShader(shader_type);

    const char *str = shader_text.c_str();
    glShaderSource(shader_id, 1, &str, nullptr);
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

static Shader::ValueType GLTypeToValueType(GLenum type) {
    switch (type) {
        case GL_FLOAT: return Shader::ValueType::FLOAT;
        case GL_INT: return Shader::ValueType::INT;
        case GL_UNSIGNED_INT: return Shader::ValueType::UNSIGNED_INT;
        case GL_FLOAT_MAT3: return Shader::ValueType::MATRIX_3_FLOAT;
        case GL_FLOAT_MAT4: return Shader::ValueType::MATRIX_4_FLOAT;
        case GL_FLOAT_VEC2: return Shader::ValueType::VECTOR_2_FLOAT;
        case GL_FLOAT_VEC3: return Shader::ValueType::VECTOR_3_FLOAT;
        case GL_FLOAT_VEC4: return Shader::ValueType::VECTOR_4_FLOAT;
        case GL_INT_VEC2: return Shader::ValueType::VECTOR_2_INT;
        case GL_INT_VEC3: return Shader::ValueType::VECTOR_3_INT;
        case GL_INT_VEC4: return Shader::ValueType::VECTOR_4_INT;
#if RENDERER_GL
        case GL_UNSIGNED_INT_VEC2: return Shader::ValueType::VECTOR_2_UNSIGNED_INT;
        case GL_UNSIGNED_INT_VEC3: return Shader::ValueType::VECTOR_3_UNSIGNED_INT;
        case GL_UNSIGNED_INT_VEC4: return Shader::ValueType::VECTOR_4_UNSIGNED_INT;
#endif
        case GL_SAMPLER_2D: return Shader::ValueType::TEXTURE_UNIT;
        default: return Shader::ValueType::UNSUPPORTED;
    }
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

    std::vector<GLchar> name_data(max_uniform_name_length > max_attribute_name_length
    ? max_uniform_name_length : max_attribute_name_length);

    GLsizei length;
    GLint size;
    GLenum type;

    GLint uniforms_count;
    glGetProgramiv(shader_id, GL_ACTIVE_UNIFORMS, &uniforms_count);
    for (GLuint i = 0; (int)i < uniforms_count; i++) {
        glGetActiveUniform(shader_id, i, name_data.size(), &length, &size, &type, &name_data[0]);
        uniforms.insert(std::make_pair(&name_data[0], Uniform {
            &name_data[0],
            GLTypeToValueType(type),
            (unsigned int)glGetUniformLocation(shader_id, &name_data[0])
        }));
    }

    GLint attributes_count;
    glGetProgramiv(shader_id, GL_ACTIVE_ATTRIBUTES, &attributes_count);
    for (GLuint i = 0; (int)i < attributes_count; i++) {
        glGetActiveAttrib(shader_id, i, name_data.size(), &length, &size, &type, &name_data[0]);
        attributes.insert(std::make_pair(&name_data[0], Attribute {
            &name_data[0],
            GLTypeToValueType(type),
            (unsigned int)glGetAttribLocation(shader_id, &name_data[0])
        }));
    }
}

Shader::~Shader() {
    ASSERT_MAIN_THREAD
    glDeleteProgram(shader_id);
}

void Shader::Bind() {
    ASSERT_MAIN_THREAD
    if (bound_shader == this) return;
    glUseProgram(shader_id);
    bound_shader = this;
}

#define SHADER_SET_UNIFORM_DEF(argument, value_type) \
void Shader::SetUniform(const std::string &name, argument v) { \
    ASSERT_MAIN_THREAD \
    auto uniform = GetUniform(name); \
    if (!bound_shader || bound_shader->shader_id != shader_id || !uniform || uniform->type != value_type) return; \
    uniform->Set(v); \
}

SHADER_SET_UNIFORM_DEF(float, Shader::ValueType::FLOAT)
SHADER_SET_UNIFORM_DEF(int, Shader::ValueType::INT)
SHADER_SET_UNIFORM_DEF(unsigned int, Shader::ValueType::TEXTURE_UNIT)
SHADER_SET_UNIFORM_DEF(const Vector2f &, Shader::ValueType::VECTOR_2_FLOAT)
SHADER_SET_UNIFORM_DEF(const Vector3f &, Shader::ValueType::VECTOR_3_FLOAT)
SHADER_SET_UNIFORM_DEF(const Vector4f &, Shader::ValueType::VECTOR_4_FLOAT)
SHADER_SET_UNIFORM_DEF(const Vector2i &, Shader::ValueType::VECTOR_2_INT)
SHADER_SET_UNIFORM_DEF(const Vector3i &, Shader::ValueType::VECTOR_3_INT)
SHADER_SET_UNIFORM_DEF(const Vector4i &, Shader::ValueType::VECTOR_4_INT)
SHADER_SET_UNIFORM_DEF(const Vector2ui &, Shader::ValueType::VECTOR_2_UNSIGNED_INT)
SHADER_SET_UNIFORM_DEF(const Vector3ui &, Shader::ValueType::VECTOR_3_UNSIGNED_INT)
SHADER_SET_UNIFORM_DEF(const Vector4ui &, Shader::ValueType::VECTOR_4_UNSIGNED_INT)
SHADER_SET_UNIFORM_DEF(const Matrix3f &, Shader::ValueType::MATRIX_3_FLOAT)
SHADER_SET_UNIFORM_DEF(const Matrix4f &, Shader::ValueType::MATRIX_4_FLOAT)

#undef SHADER_SET_UNIFORM_DEF

const std::map<std::string, Shader::Uniform> &Shader::GetUniforms() const {
    return uniforms;
}

Shader::Uniform *Shader::GetUniform(const std::string &name) const {
    auto u = uniforms.find(name);
    if (u != uniforms.end()) return const_cast<Uniform *>(&u->second);
    return nullptr;
}

const std::map<std::string, Shader::Attribute> &Shader::GetAttributes() const {
    return attributes;
}

Shader::Attribute *Shader::GetAttribute(const std::string &name) const {
    auto u = attributes.find(name);
    if (u != attributes.end()) return const_cast<Attribute *>(&u->second);
    return nullptr;
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
#if RENDERER_GL
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

void Shader::Uniform::Set(const Vector2i &vec) {
    ASSERT_MAIN_THREAD
    glUniform2i(location, vec.x, vec.y);
}

void Shader::Uniform::Set(const Vector3i &vec) {
    ASSERT_MAIN_THREAD
    glUniform3i(location, vec.x, vec.y, vec.z);
}

void Shader::Uniform::Set(const Vector4i &vec) {
    ASSERT_MAIN_THREAD
    glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::Uniform::Set(const Vector2ui &vec) {
    ASSERT_MAIN_THREAD
#if RENDERER_GL
    glUniform2ui(location, vec.x, vec.y);
#elif RENDERER_GL_ES
    glUniform2i(location, vec.x, vec.y);
#endif
}

void Shader::Uniform::Set(const Vector3ui &vec) {
    ASSERT_MAIN_THREAD
#if RENDERER_GL
    glUniform3ui(location, vec.x, vec.y, vec.z);
#elif RENDERER_GL_ES
    glUniform3i(location, vec.x, vec.y, vec.z);
#endif
}

void Shader::Uniform::Set(const Vector4ui &vec) {
    ASSERT_MAIN_THREAD
#if RENDERER_GL
    glUniform4ui(location, vec.x, vec.y, vec.z, vec.w);
#elif RENDERER_GL_ES
    glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
#endif
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
    if (!bound_shader) return;
    glUseProgram(0);
    bound_shader = nullptr;
}

intrusive_ptr<Shader> Shader::Create(const std::string &vertex_shader_text, const std::string &fragment_shader_text) {
    return new Shader(vertex_shader_text, fragment_shader_text);
}

} //namespace a2d