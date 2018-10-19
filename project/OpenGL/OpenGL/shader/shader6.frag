#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float textureAlpha;

void main()
{
	//���������ɫ��������������Ľ�ϡ�GLSL�ڽ���mix������Ҫ��������ֵ��Ϊ�������������Ǹ��ݵ����������������Բ�ֵ��
	//���������ֵ��0.0�����᷵�ص�һ�����룻�����1.0���᷵�صڶ�������ֵ��
	//0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ����������������Ļ��ɫ

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);


	//ʹ���ⲿ�����Ķ�����ɫ���
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = color * vec4(ourColor,1.0);


	//�޸�����texture2��ˮƽ��ת
	//vec2 textcoord2 = vec2(-TexCoord.x,TexCoord.y);
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, textcoord2), 0.2);
	//FragColor = color * vec4(ourColor,1.0);

	//ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ���
	vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	FragColor = color * vec4(ourColor,1.0);
}