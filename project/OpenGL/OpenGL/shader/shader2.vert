#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0); //注意我们如何把一个vec3作为vec4的构造器的参数
}