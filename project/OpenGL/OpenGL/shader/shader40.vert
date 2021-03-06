#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aColor;		// 颜色变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;	// 纹理坐标变量的属性位置值为 2

out vec3 ourColor; // 向片段着色器输出一个颜色
out vec2 TexCoord;

out vec3 FragPos;

uniform mat4 model;

void main()
{
	gl_Position = vec4(aPos, 1.0);
    ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色
    TexCoord = aTexCoord;

	FragPos = vec3(model * vec4(aPos, 1.0f)); //乘以模型矩阵把顶点坐标转到世界空间里
}