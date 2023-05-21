#version 400

layout(location=0) in vec3 coord3d;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(coord3d,1.0);
}
