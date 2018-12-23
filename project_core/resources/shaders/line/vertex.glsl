#ifdef RENDERER_GL_ES

precision highp float;
#define IN attribute
#define OUT varying

#else

#define IN in
#define OUT out

#endif

uniform mat4 camera_matrix;
uniform mat4 transform_matrix;

#ifdef RENDERER_GL
layout(location = 0)
#endif
IN vec2 position;

#ifdef RENDERER_GL
layout(location = 2)
#endif
IN vec4 color;

OUT vec4 color_;

void main() {
    color_ = color;
    gl_Position = camera_matrix * transform_matrix * vec4(position.x, position.y, 0, 1);
}
