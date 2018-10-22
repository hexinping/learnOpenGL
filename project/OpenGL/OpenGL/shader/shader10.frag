#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float textureAlpha;


uniform vec3 lightColor; //�����ɫ

void main()
{
	//ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ���
	vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	FragColor =  vec4(color.xyz * lightColor,1.0);

}