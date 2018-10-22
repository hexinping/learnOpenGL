#version 330 core
layout (location = 0) in vec3 aPos;			// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor;		// ��ɫ����������λ��ֵΪ 1
layout (location = 2) in vec2 aTexCoord;	// �����������������λ��ֵΪ 2

out vec2 TexCoord;
out vec3 ourColor; // ��Ƭ����ɫ�����һ����ɫ

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
	ourColor = aColor; // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
	TexCoord = aTexCoord;


    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //ˮƽ��ת
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //��ֱ��ת
}