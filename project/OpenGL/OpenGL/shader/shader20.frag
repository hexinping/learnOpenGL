#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

const float offset = 1.0 / 300.0;  

//反色
void func1()
{
	// 像素的颜色值范围为（0,1），反色起始就是把每个像素的颜色值（rgb）取反
	vec3 col = texture(texture1, TexCoords).rgb;
	FragColor = vec4(1.0-col, 1.0);
}


//灰度1
void func2()
{
	//像素颜色rgb值，取中间值
	FragColor = texture(texture1, TexCoords);
	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
	FragColor = vec4(average, average, average, 1.0);
}

//灰度2
void func3()
{
	FragColor = texture(texture1, TexCoords);
	float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	FragColor = vec4(average, average, average, 1.0);

}


//核效果
void func4()
{
   vec2 offsets[9] = vec2[](
	    vec2(-offset,  offset), // 左上
	    vec2( 0.0f,    offset), // 正上
	    vec2( offset,  offset), // 右上
	    vec2(-offset,  0.0f),   // 左
	    vec2( 0.0f,    0.0f),   // 中
	    vec2( offset,  0.0f),   // 右
	    vec2(-offset, -offset), // 左下
	    vec2( 0.0f,   -offset), // 正下
	    vec2( offset, -offset)  // 右下
	);

	float kernel[9] = float[](
	    -1, -1, -1,
	    -1,  9, -1,
	    -1, -1, -1
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
	    sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];

	FragColor = vec4(col, 1.0);

}


//模糊
void func5()
{
   vec2 offsets[9] = vec2[](
	    vec2(-offset,  offset), // 左上
	    vec2( 0.0f,    offset), // 正上
	    vec2( offset,  offset), // 右上
	    vec2(-offset,  0.0f),   // 左
	    vec2( 0.0f,    0.0f),   // 中
	    vec2( offset,  0.0f),   // 右
	    vec2(-offset, -offset), // 左下
	    vec2( 0.0f,   -offset), // 正下
	    vec2( offset, -offset)  // 右下
	);

	float kernel[9] = float[](
	    1.0 / 16, 2.0 / 16, 1.0 / 16,
	    2.0 / 16, 4.0 / 16, 2.0 / 16,
	    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
	    sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];

	FragColor = vec4(col, 1.0);

}


//边缘检测
void func6()
{
   vec2 offsets[9] = vec2[](
	    vec2(-offset,  offset), // 左上
	    vec2( 0.0f,    offset), // 正上
	    vec2( offset,  offset), // 右上
	    vec2(-offset,  0.0f),   // 左
	    vec2( 0.0f,    0.0f),   // 中
	    vec2( offset,  0.0f),   // 右
	    vec2(-offset, -offset), // 左下
	    vec2( 0.0f,   -offset), // 正下
	    vec2( offset, -offset)  // 右下
	);

	float kernel[9] = float[](
	     1, 1, 1,
	     1, -8, 1,
	     1, 1, 1
	 );

	vec3 sampleTex[9];
	for(int i = 0; i < 9; i++)
	{
	    sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];

	FragColor = vec4(col, 1.0);

}


void main()
{             
	vec4 texColor = texture(texture1, TexCoords);
	if(texColor.a < 0.2)
		discard;
	//FragColor = texColor;

	func1();
	//func2();
	//func3();
	//func4();
	//func5();
	//func6();

}