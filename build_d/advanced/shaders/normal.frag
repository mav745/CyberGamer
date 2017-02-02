#version 330

in vec2 coord;
in vec3 vcolor;

out vec4 color;

uniform sampler2D tex;
// uniform vec4 col;

void main()
{
	// color = texture2D(tex, coord.st)*col;
	color = texture2D(tex, coord.st) * vec4(vcolor, 1.0);
}
