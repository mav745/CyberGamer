#version 330

in vec3 pos;
in vec3 col;
in vec2 uv;

out vec2 coord;
out vec3 vcolor;

uniform mat4 mv;
uniform mat4 proj;

void main()
{
	vec4 mvPosition = mv * vec4( pos, 1.0 );
	gl_Position     = proj * mvPosition;
	coord  = uv;
	vcolor = col;
}
