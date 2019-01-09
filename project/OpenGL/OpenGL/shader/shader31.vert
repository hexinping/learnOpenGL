#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 
layout (location = 1) in vec2 aTexCoords;	// 纹理坐标变量的属性位置值为 2
layout (location = 2) in vec3 aNormal;      // 法线
layout (location = 3) in vec3 aTangent;     // 切线
layout (location = 4) in vec3 aBitangent;   // 副切线



//我们直接使用TBN矩阵，这个矩阵可以把切线坐标空间的向量转换到世界坐标空间。
//因此我们把它传给片段着色器中，把通过采样得到的法线坐标左乘上TBN矩阵，
//转换到世界坐标空间中，这样所有法线和其他光照变量就在同一个坐标系中了

out vec2 TexCoords;

out vec3 Normal;
out vec3 FragPos;  //世界空间的坐标
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;

	Normal = mat3(transpose(inverse(model))) * aNormal;// 使用法线矩阵，避免不等比缩放造成的法线不垂直与表面，来保证它失去了位移属性以及能够乘以vec3的法向量。

	FragPos = vec3(model * vec4(aPos, 1.0f)); //乘以模型矩阵把顶点坐标转到世界空间里

	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    TBN = mat3(T, B, N); //TBN为世界空间

    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //水平翻转
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //竖直翻转
}