#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float textureAlpha;

void main()
{
	//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
}