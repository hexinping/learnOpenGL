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

float lookup(vec2 p, float dx, float dy)
{
	//取一个像素周边像素的颜色值 ，然后取一个灰度值
    vec2 uv = p.xy + vec2(dx , dy ) / resolution.xy;
    vec4 c = texture2D(texture1, uv.xy);

	//根据当前像素取一个灰度值
    return 0.2126*c.r + 0.7152*c.g + 0.0722*c.b;
}

//边缘检测
void func9()
{
	 vec2 p = TexCoords.xy;
    // simple sobel edge detection
    float gx = 0.0;
    gx += -1.0 * lookup(p, -1.0, -1.0);
    gx += -2.0 * lookup(p, -1.0,  0.0);
    gx += -1.0 * lookup(p, -1.0,  1.0);
    gx +=  1.0 * lookup(p,  1.0, -1.0);
    gx +=  2.0 * lookup(p,  1.0,  0.0);
    gx +=  1.0 * lookup(p,  1.0,  1.0);
    
    float gy = 0.0;
    gy += -1.0 * lookup(p, -1.0, -1.0);
    gy += -2.0 * lookup(p,  0.0, -1.0);
    gy += -1.0 * lookup(p,  1.0, -1.0);
    gy +=  1.0 * lookup(p, -1.0,  1.0);
    gy +=  2.0 * lookup(p,  0.0,  1.0);
    gy +=  1.0 * lookup(p,  1.0,  1.0);
    
    float g = gx*gx + gy*gy;
    
    FragColor.xyz = vec3(1.-g);
    FragColor.w = 1.;
}

//简易灰度
void func10()
{
	vec4 c = texture2D(texture1, TexCoords);
	FragColor.xyz = vec3(0.2126*c.r + 0.7152*c.g + 0.0722*c.b);
	FragColor.w = c.w;
}

//
void func11()
{
	vec4 c = texture2D(texture1, TexCoords);
    vec4 final = c;
    final.r = (c.r * 0.393) + (c.g * 0.769) + (c.b * 0.189);
    final.g = (c.r * 0.349) + (c.g * 0.686) + (c.b * 0.168);
    final.b = (c.r * 0.272) + (c.g * 0.534) + (c.b * 0.131);

	FragColor = final;
}

const float blurSize_bloom = 1.0/512.0;
const float intensity_bloom = 0.35;
//bloom 效果: 水平模糊+垂直模糊
void func12()
{
	   vec4 sum = vec4(0);
	   vec2 texcoord = TexCoords.xy;
 
	   //thank you! http://www.gamerendering.com/2008/10/11/gaussian-blur-filter-shader/ for the 
	   //blur tutorial
	   // blur in y (vertical)
	   // take nine samples, with the distance blurSize between them
	   sum += texture2D(texture1, vec2(texcoord.x - 4.0*blurSize_bloom, texcoord.y)) * 0.05;
	   sum += texture2D(texture1, vec2(texcoord.x - 3.0*blurSize_bloom, texcoord.y)) * 0.09;
	   sum += texture2D(texture1, vec2(texcoord.x - 2.0*blurSize_bloom, texcoord.y)) * 0.12;
	   sum += texture2D(texture1, vec2(texcoord.x - blurSize_bloom, texcoord.y)) * 0.15;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y)) * 0.16;
	   sum += texture2D(texture1, vec2(texcoord.x + blurSize_bloom, texcoord.y)) * 0.15;
	   sum += texture2D(texture1, vec2(texcoord.x + 2.0*blurSize_bloom, texcoord.y)) * 0.12;
	   sum += texture2D(texture1, vec2(texcoord.x + 3.0*blurSize_bloom, texcoord.y)) * 0.09;
	   sum += texture2D(texture1, vec2(texcoord.x + 4.0*blurSize_bloom, texcoord.y)) * 0.05;
	
		// blur in y (vertical)
	   // take nine samples, with the distance blurSize between them
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y - 4.0*blurSize_bloom)) * 0.05;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y - 3.0*blurSize_bloom)) * 0.09;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y - 2.0*blurSize_bloom)) * 0.12;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y - blurSize_bloom)) * 0.15;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y)) * 0.16;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y + blurSize_bloom)) * 0.15;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y + 2.0*blurSize_bloom)) * 0.12;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y + 3.0*blurSize_bloom)) * 0.09;
	   sum += texture2D(texture1, vec2(texcoord.x, texcoord.y + 4.0*blurSize_bloom)) * 0.05;

	   //increase blur with intensity!
	   FragColor = sum*intensity_bloom + texture2D(texture1, texcoord); 
}


#define FILTER_SIZE 3
#define COLOR_LEVELS 7.0
#define EDGE_FILTER_SIZE 3
#define EDGE_THRESHOLD 0.05

vec4 edgeFilter(in int px, in int py)
{
	vec4 color = vec4(0.0);
	
	for (int y = -EDGE_FILTER_SIZE; y <= EDGE_FILTER_SIZE; ++y)
	{
		for (int x = -EDGE_FILTER_SIZE; x <= EDGE_FILTER_SIZE; ++x)
		{
			color += texture2D(texture1, TexCoords + vec2(px + x, py + y) / resolution.xy);
		}
	}

	color /= float((2 * EDGE_FILTER_SIZE + 1) * (2 * EDGE_FILTER_SIZE + 1));
	
	return color;
}


void func13()
{
	// Shade
	vec4 color = vec4(0.0);
	
	for (int y = -FILTER_SIZE; y <= FILTER_SIZE; ++y)
	{
		for (int x = -FILTER_SIZE; x <= FILTER_SIZE; ++x)
		{
			color += texture2D(texture1, TexCoords + vec2(x, y) / resolution.xy);
		}
	}

	color /= float((2 * FILTER_SIZE + 1) * (2 * FILTER_SIZE + 1));
	
	for (int c = 0; c < 3; ++c)
	{
		color[c] = floor(COLOR_LEVELS * color[c]) / COLOR_LEVELS;
	}
	
	// Highlight edges
	vec4 sum = abs(edgeFilter(0, 1) - edgeFilter(0, -1));
	sum += abs(edgeFilter(1, 0) - edgeFilter(-1, 0));
	sum /= 2.0;	

	if (length(sum) > EDGE_THRESHOLD)
	{
		color.rgb = vec3(0.0);	
	}
	
	FragColor = color;
}



// 返回对应像素的纹理颜色
float noise_LensFlare(float t)
{
	return texture2D(texture1,vec2(t,.0)/resolution.xy).x;
}
float noise_LensFlare(vec2 t)
{
	return texture2D(texture1,t/resolution.xy).x;
}

vec3 lensflare(vec2 uv,vec2 pos)
{
	vec2 main = uv-pos;
	vec2 uvd = uv*(length(uv));
	
	float ang = atan(main.x,main.y);
	float dist=length(main); dist = pow(dist,.1);
	float n = noise_LensFlare(vec2(ang*16.0,dist*32.0));
	
	float f0 = 1.0/(length(uv-pos)*16.0+1.0);
	
	f0 = f0+f0*(sin(noise_LensFlare((pos.x+pos.y)*2.2+ang*4.0+5.954)*16.0)*.1+dist*.1+.8);
	
	float f1 = max(0.01-pow(length(uv+1.2*pos),1.9),.0)*7.0;
    
	float f2 = max(1.0/(1.0+32.0*pow(length(uvd+0.8*pos),2.0)),.0)*00.25;
	float f22 = max(1.0/(1.0+32.0*pow(length(uvd+0.85*pos),2.0)),.0)*00.23;
	float f23 = max(1.0/(1.0+32.0*pow(length(uvd+0.9*pos),2.0)),.0)*00.21;
	
	vec2 uvx = mix(uv,uvd,-0.5);
	
	float f4 = max(0.01-pow(length(uvx+0.4*pos),2.4),.0)*6.0;
	float f42 = max(0.01-pow(length(uvx+0.45*pos),2.4),.0)*5.0;
	float f43 = max(0.01-pow(length(uvx+0.5*pos),2.4),.0)*3.0;
	
	uvx = mix(uv,uvd,-.4);
	
	float f5 = max(0.01-pow(length(uvx+0.2*pos),5.5),.0)*2.0;
	float f52 = max(0.01-pow(length(uvx+0.4*pos),5.5),.0)*2.0;
	float f53 = max(0.01-pow(length(uvx+0.6*pos),5.5),.0)*2.0;
	
	uvx = mix(uv,uvd,-0.5);
	
	float f6 = max(0.01-pow(length(uvx-0.3*pos),1.6),.0)*6.0;
	float f62 = max(0.01-pow(length(uvx-0.325*pos),1.6),.0)*3.0;
	float f63 = max(0.01-pow(length(uvx-0.35*pos),1.6),.0)*5.0;
	
	vec3 c = vec3(.0);
	
	c.r+=f2+f4+f5+f6; c.g+=f22+f42+f52+f62; c.b+=f23+f43+f53+f63;
	c = c*1.3 - vec3(length(uvd)*.05);
	c+=vec3(f0);
	
	return c;
}

vec3 cc(vec3 color, float factor,float factor2) // color modifier
{
	float w = color.x+color.y+color.z;
	return mix(color,vec3(w)*factor,w*factor2);
}

void func14()
{
	vec2 uv = TexCoords - 0.5;
	uv.x *= resolution.x/resolution.y; //fix aspect ratio
    vec3 mouse;
    mouse.z = 0.5;
    mouse.x=sin(Time)*.5;
    mouse.y=sin(Time*.913)*.5;
	
	vec3 color = vec3(1.4,1.2,1.0)*lensflare(uv,mouse.xy);
	color -= noise_LensFlare(TexCoords * resolution)*.015;
	color = cc(color,.5,.1);
	FragColor = vec4(color,1.0);

}



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
	//func8();
	//func9();
	//func10();
	//func11();
	//func12();
	//func13();

	func14();

}