#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

//颜色矩阵
uniform mat4 u_colorMultiplier;
uniform vec4 u_colorOffset;

void main()
{
	//texture(ourTexture, TexCoord) 返回的是一个vec4

    vec4 color = texture(ourTexture, TexCoord); //直接用纹理的颜色最终输出
	if(color.a < 0.5)
	{
		discard;
	}

	color = u_colorMultiplier * color + u_colorOffset;

	FragColor = color;
	//FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); //用纹理的颜色*顶点颜色最终输出
}