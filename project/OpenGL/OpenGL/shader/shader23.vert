#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos; //顶点可以直接当做纹理坐标
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; //这样的目的是为了在透视除法下/w,顶点z的值保持为1，这样就最远了
}