#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;  //世界空间的坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 lightPos;  
uniform vec3 viewPos;  // 摄像机的位置

out mat3 TBN;  //TBN逆矩阵
//out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoords = aTexCoords;

	Normal = mat3(transpose(inverse(model))) * aNormal;// 使用法线矩阵，避免不等比缩放造成的法线不垂直与表面，来保证它失去了位移属性以及能够乘以vec3的法向量。

	FragPos = vec3(model * vec4(aPos, 1.0f)); //乘以模型矩阵把顶点坐标转到世界空间里

	vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    //vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
	
	//再次正交化，避免出现TBN向量不垂直
	T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T); //有了两个垂直的向量就叉乘就能得出第三个相互垂直的向量了，可以不用传副切线aBitangent
    

	//注意，这里我们使用transpose函数，而不是inverse函数。
	//正交矩阵（每个轴既是单位向量同时相互垂直）的一大属性是一个正交矩阵的置换矩阵与它的逆矩阵相等。
	//这个属性和重要因为逆矩阵的求得比求置换开销大；结果却是一样的

	TBN = transpose(mat3(T, B, N)); //切线空间矩阵的逆矩阵

	//TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;


    //TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);  //水平翻转
	//TexCoord = vec2(1.0-aTexCoord.x, aTexCoord.y);   //竖直翻转
}