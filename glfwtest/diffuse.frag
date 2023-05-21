#version 400

in vec3 outColor;

layout( location = 0 )  out vec4 fragColor;

void main()
{
	fragColor = vec4(outColor,1.0);
}