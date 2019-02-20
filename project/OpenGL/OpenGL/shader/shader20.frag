#version 330 core
out vec4 FragColor;  // 直接用gl_FragColor也可以 默认就输出这个

//  这里肯定有朋友会问，一个for循环就搞定啦，怎么这么麻烦！其实我一开始也是用for的，但后来在安卓某些机型（如小米4）会直接崩溃，
//  查找资料发现OpenGL es并不是很支持循环，while和for都不要用

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec2 resolution;

const float offset = 1.0 / 100.0;  
uniform float Time;

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

	//移动设备上for循环支持不太好，可以展开 todo...
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


//模糊：原理就是用该像素周边的像素与自己做平均值插值颜色
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
	//移动设备上for循环支持不太好，可以展开 todo...
	for(int i = 0; i < 9; i++)
	{
	    sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i])); //取到周边像素的纹理颜色
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i]; //做平均值计算

	FragColor = vec4(col, 1.0);

}

vec4 blur(vec2 p)
{
    vec4 col = vec4(0);
    vec2 unit = 1.0 / resolution.xy * 2;
        
    float count = 0.0;
        
    for(float x = -4.0; x <= 4.0; x += 2.0)
    {
        for(float y = -4.0; y <= 4.0; y += 2.0)
        {
            float weight = (4.0 - abs(x)) * (4.0 - abs(y)); //计算权重
            col += texture2D(texture1, p + vec2(x * unit.x *1.5, y * unit.y * 1.5)) * weight; //去周边的像素*权重 计算最后的颜色值
            count += weight;
        }
    }
        
    return col / count;
}

//模糊2
void func5_1()
{
	vec4 col = blur(TexCoords);
    FragColor = col;
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

	 //移动设备上for循环支持不太好，可以展开 todo...
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

//描边1
void func7()
{
	
    float radius = 0.01;
    vec4 accum = vec4(0.0);
    vec4 normal = vec4(0.0);
    
    normal = texture2D(texture1, vec2(TexCoords.x, TexCoords.y)); //取到纹理的颜色
    
	// 取四周纹理的颜色进行均值操作
    accum += texture2D(texture1, vec2(TexCoords.x - radius, TexCoords.y - radius));
    accum += texture2D(texture1, vec2(TexCoords.x + radius, TexCoords.y - radius));
    accum += texture2D(texture1, vec2(TexCoords.x + radius, TexCoords.y + radius));
    accum += texture2D(texture1, vec2(TexCoords.x - radius, TexCoords.y + radius));
    
    accum *= 1.75;

	vec3 outLineColor = vec3(1.0f, 0.2f, 0.3f);
    accum.rgb =  outLineColor * accum.a;
    accum.a = 1.0;
    
    vec4 result = ( accum * (1.0 - normal.a)) + (normal * normal.a);  //根据当前像素的透明度来决定最后插值颜色
    
    FragColor = result;
}


uniform vec2 textureSize; // 纹理大小（宽和高），为了计算周围各点的纹理坐标，必须传入它，因为纹理坐标范围是0~1  

// 判断在这个角度上距离为outlineSize那一点是不是透明  
int getIsStrokeWithAngel(float angel,float outlineSize)  
{  
    int stroke = 0;  
    float rad = angel * 0.01745329252; // 这个浮点数是 pi / 180，角度转弧度  

	// 这句比较难懂，outlineSize * cos(rad)可以理解为在x轴上投影，除以textureSize.x是因为texture2D接收的是一个0~1的纹理坐标，而不是像素坐标  
    float a = texture2D(texture1, vec2(TexCoords.x + outlineSize * cos(rad) / textureSize.x, TexCoords.y + outlineSize * sin(rad) / textureSize.y)).a; 
    if (a >= 0.8)// 我把alpha值大于0.8都视为不透明，小于0.5都视为透明  
    {  
        stroke = 1;  
    }  
    return stroke;  
}  
  

//描边2： 1. 如果它是不透明的像素，则不管，维持原本颜色；2. 如果透明，是360度判断它四周有没有不透明的像素，如果有，则把它设成描边颜色，否则保持透明。
void func7_1(vec4 color)
{

	if (color.a >= 0.8) // 不透明，不管，直接返回  
    {  
        FragColor = color;  
        return;  
    }  

	vec3 outlineColor = vec3(1.0,0,0);				// 描边颜色  
	float outlineSize = 5.0;						// 描边宽度，以像素为单位

	//360度判读四周有没有透明像素
	int strokeCount = 0;  
    strokeCount += getIsStrokeWithAngel(0.0,  outlineSize);  
    strokeCount += getIsStrokeWithAngel(30.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(60.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(90.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(120.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(150.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(180.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(210.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(240.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(270.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(300.0, outlineSize);  
    strokeCount += getIsStrokeWithAngel(330.0, outlineSize);  

	if (strokeCount > 0) // 四周围至少有一个点是不透明的，这个点要设成描边颜色  
    {  
        color.rgb = outlineColor;  
        color.a = 1.0;  
    }  
  
    FragColor = color; 

}


const float intensity = 0.05;
vec3 noise(vec2 uv)
{
	vec2 p = abs(sin(uv * 13.0 + uv.x * Time * sin(uv.y)));
	
	return vec3(sin (0.2 * Time + sin(p * 0.5) * Time / cos(50.0)) * 10.0,0.3+0.5 * abs(sin(Time * tan(5.0))));

}

// 噪点效果
void func8()
{
	vec2 u = gl_FragCoord.xy/sin(resolution.xy * Time * 0.01);
	vec3 color = intensity * noise(u) + (1-intensity)*texture2D(texture1,TexCoords.xy).xyz;

	FragColor =  vec4(color,1.0);

}

//"Shaders/example_EdgeDetection.fsh"  -- Edge Detect
// "Shaders/example_GreyScale.fsh"     --  Grey
// "Shaders/example_Sepia.fsh"         -- Sepia
// "Shaders/example_Bloom.fsh"         -- bloom  
//  "Shaders/example_CelShading.fsh"   -- cel shading
// "Shaders/example_LensFlare.fsh"    -- Lens Flare  
void main()
{             
	vec4 texColor = texture(texture1, TexCoords);
	if(texColor.a < 0.2)
		discard;
	//FragColor = texColor;

	//func1();
	//func2();
	//func3();
	//func4();
	//func5();
	//func5_1();
	//func6();

	//func7();
	//func7_1(texColor);
	func8();

}