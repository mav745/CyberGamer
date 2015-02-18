#version 330
in vec3 pos;in vec2 uv;
out vec2 coord;
uniform mat4 mat;
void main()
{    gl_Position = mat*vec4(pos, 1.0);    coord = uv;}