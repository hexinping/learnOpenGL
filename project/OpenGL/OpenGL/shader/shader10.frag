#version 330 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float textureAlpha;


uniform vec3 lightColor; //光的颜色

void main()
{
	//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度
	vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	FragColor =  vec4(color.xyz * lightColor,1.0);

}