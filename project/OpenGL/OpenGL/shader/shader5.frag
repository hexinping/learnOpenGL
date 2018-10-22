#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
	//texture(ourTexture, TexCoord) 返回的是一个vec4

    //FragColor = texture(ourTexture, TexCoord); //直接用纹理的颜色最终输出
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); //用纹理的颜色*顶点颜色最终输出
}