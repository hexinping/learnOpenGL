#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset; //实例化数组

out vec3 fColor;

//uniform vec2 offsets[100];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   // vec2 offset = offsets[gl_InstanceID];
	gl_Position = projection * view * model * vec4(aPos + aOffset, 0.0, 1.0f);
    fColor = aColor;
}