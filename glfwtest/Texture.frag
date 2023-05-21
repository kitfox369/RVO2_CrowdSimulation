#version 400

//vertex shader를 이용하여 나온 정점들을 이엉서 primitive가 생성
//rasterizer를 이용해야 primitive에 해당하는 모든 fragment가 결정됨
//이러한 fragment들에 대한 작업을 수행하는 것

in vec3 f_color;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 FragColors;

void main()
{

	FragColors = vec4(f_color,1.0)*texture(ourTexture,TexCoord);
   //FragColors = vec4(f_color, 1.0);
}