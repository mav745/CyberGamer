#version 330

in vec2 coord;

out vec4 color;

uniform sampler2D tex;
uniform vec4 col;

void main()
{
    color = texture2D(tex, coord.st)*col;
}