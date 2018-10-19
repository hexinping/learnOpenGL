#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	//最终输出颜色现在是两个纹理的结合。GLSL内建的mix函数需要接受两个值作为参数，并对它们根据第三个参数进行线性插值。
	//如果第三个值是0.0，它会返回第一个输入；如果是1.0，会返回第二个输入值。
	//0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor = color * vec4(ourColor,1.0);
}