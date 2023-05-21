#version 400

layout(location=0) in vec3 coord3d;
layout(location=1) in vec3 v_color;

out vec3 outColor;

uniform vec4 LightLocation;
uniform vec3 Kd;
uniform vec3 Ld;

uniform vec3 color;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 mvp;
uniform mat4 location;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//change vertex position : 모든 light 계산은 camera좌표계에서 이루어짐
	vec4 P= ModelViewMatrix * vec4(coord3d,1.0); //camera좌표계에서의 vertex위치값
	//normal 변경
	vec3 N = normalize(NormalMatrix * v_color); //local 좌표계에서의 normal을 변경
	
	vec3 L = normalize(vec3(LightLocation - P));	//내적계산을 위해 vec3로 변경

	outColor = color;

	gl_Position = projection* view *location* model * vec4(coord3d,1.0);

}
