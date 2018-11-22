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
uniform vec2 uv_lb;
uniform vec2 uv_rt;
uniform float ratio;

IN vec2 position;

OUT vec2 uv;

void main() {
    uv = uv_lb + (position.xy + 0.5) * (uv_rt - uv_lb);
    gl_Position = camera_matrix * model_matrix * vec4(position.x * ratio, position.y, 0, 1);
}
