#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor;		// 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;	// 纹理坐标变量的属性位置值为 2

out vec2 TexCoord;
out vec3 ourColor; // 向片段着色器输出一个颜色

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
	TexCoord = aTexCoord;


    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //水平翻转
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //竖直翻转
}