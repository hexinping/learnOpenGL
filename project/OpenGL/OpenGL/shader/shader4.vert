#version 330 core
layout (location = 0) in vec3 aPos;

uniform float ourPos; // 在OpenGL程序代码中设定这个变量

void main()
{

    gl_Position = vec4(aPos.x + ourPos, aPos.y, aPos.z, 1.0); //注意我们如何把一个vec3作为vec4的构造器的参数
}