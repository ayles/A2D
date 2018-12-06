#ifdef RENDERER_GL_ES

precision highp float;
#define IN attribute
#define OUT varying

#else

#define IN in
#define OUT out

#endif

uniform mat4 camera_matrix;

#ifdef RENDERER_GL
layout(location = 0)
#endif
IN vec2 position;

#ifdef RENDERER_GL
layout(location = 1)
#endif
IN vec2 uv;

OUT vec2 uv_;

void main() {
    uv_ = uv;
    gl_Position = camera_matrix * vec4(position.x, position.y, 0, 1);
}
