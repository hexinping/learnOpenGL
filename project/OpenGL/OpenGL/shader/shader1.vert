#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0); //注意我们如何把一个vec3作为vec4的构造器的参数
	vertexColor = vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
}