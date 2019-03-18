#version 330 core
layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1

uniform float xOffset;

void main()
{
    gl_Position = vec4(aPos.x+ xOffset, aPos.y,aPos.z,1.0);

	//gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0); // just add a - to the y position  位置上下颠倒
    //ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
}