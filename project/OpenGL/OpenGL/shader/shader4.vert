#version 330 core
layout (location = 0) in vec3 aPos;

uniform float ourPos; // ��OpenGL����������趨�������

void main()
{

    gl_Position = vec4(aPos.x + ourPos, aPos.y, aPos.z, 1.0); //ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
}