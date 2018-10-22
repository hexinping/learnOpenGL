#version 330 core
layout (location = 0) in vec3 aPos;			// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec2 aTexCoords;	// �����������������λ��ֵΪ 2
layout (location = 2) in vec3 aNormal;      // ����

out vec2 TexCoords;

out vec3 Normal;
out vec3 FragPos;  //����ռ������

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;

	Normal = mat3(transpose(inverse(model))) * aNormal;// ʹ�÷��߾��󣬱��ⲻ�ȱ�������ɵķ��߲���ֱ����棬����֤��ʧȥ��λ�������Լ��ܹ�����vec3�ķ�������

	FragPos = vec3(model * vec4(aPos, 1.0f)); //����ģ�;���Ѷ�������ת������ռ���

    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //ˮƽ��ת
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //��ֱ��ת
}