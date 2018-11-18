#ifdef RENDERER_GL_ES

precision highp float;
#define IN attribute
#define OUT varying

#else

#define IN in
#define OUT out

#endif

uniform mat4 camera_matrix;
uniform mat4 model_matrix;
uniform vec2 texture_offset;
uniform vec2 texture_size;

IN vec2 position;

OUT vec2 uv;

void main() {
    uv = (position.xy + 0.5) * texture_size + texture_offset;
    gl_Position = camera_matrix * model_matrix * vec4(position.x, position.y, 0, 1);
}
