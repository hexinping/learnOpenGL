#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 
layout (location = 1) in vec2 aTexCoords;	// 纹理坐标变量的属性位置值为 2
layout (location = 2) in vec3 aNormal;      // 法线

out vec2 TexCoords;

out vec3 Normal;
out vec3 FragPos;  //世界空间的坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool inverse_normals;


void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;

	vec3 n = inverse_normals ? -aNormal : aNormal;

	Normal = mat3(transpose(inverse(model))) * n;// 使用法线矩阵，避免不等比缩放造成的法线不垂直与表面，来保证它失去了位移属性以及能够乘以vec3的法向量。

	FragPos = vec3(model * vec4(aPos, 1.0f)); //乘以模型矩阵把顶点坐标转到世界空间里

    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //水平翻转
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //竖直翻转
}