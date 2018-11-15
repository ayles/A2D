#ifdef ES

precision highp float;
#define IN attribute
#define OUT varying
#define TEXTURE texture2D

#else

#define IN in
#define OUT out
#define TEXTURE2D texture

#endif

uniform mat4 camera_matrix;
uniform mat4 model_matrix;

IN vec2 position;

OUT vec2 uv;

void main() {
    uv = position.xy + 0.5;
    gl_Position = camera_matrix * model_matrix * vec4(position.x, position.y, 0, 1);
}
