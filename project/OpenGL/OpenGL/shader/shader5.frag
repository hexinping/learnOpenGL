#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
	//texture(ourTexture, TexCoord) ���ص���һ��vec4

    //FragColor = texture(ourTexture, TexCoord); //ֱ�����������ɫ�������
	FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); //���������ɫ*������ɫ�������
}