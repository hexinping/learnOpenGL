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

	   //vec4 color = texture2D(texture1, texcoord);
	   //const float colorThreshold = 0.12;
	   //color = clamp(color - colorThreshold, 0.0, 1.0);
	   //gl_FragColor = sum*intensity + color; 
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


//浮雕效果
void func15()
{
	const vec2 texOffset = vec2( 0.005, 0.005);
	const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);
	vec2 tc0 = TexCoords.st + vec2(-texOffset.s, -texOffset.t);
    vec2 tc1 = TexCoords.st + vec2(         0.0, -texOffset.t);
    vec2 tc2 = TexCoords.st + vec2(-texOffset.s,          0.0);
    vec2 tc3 = TexCoords.st + vec2(+texOffset.s,          0.0);
    vec2 tc4 = TexCoords.st + vec2(         0.0, +texOffset.t);
    vec2 tc5 = TexCoords.st + vec2(+texOffset.s, +texOffset.t);

    vec4 col0 = texture2D(texture1, tc0);
    vec4 col1 = texture2D(texture1, tc1);
    vec4 col2 = texture2D(texture1, tc2);
    vec4 col3 = texture2D(texture1, tc3);
    vec4 col4 = texture2D(texture1, tc4);
    vec4 col5 = texture2D(texture1, tc5);

    vec4 sum = vec4(0.5) + (col0 + col1 + col2) - (col3 + col4 + col5);
    float lum = dot(sum, lumcoeff);
    FragColor = vec4(lum, lum, lum, 1.0);
}


//黑白效果
void func16()
{
	 vec4 col = texture2D(texture1, TexCoords.st);
	 const vec4 W = vec4(0.2125, 0.7154, 0.0721, 0);
     float lum = dot(col, W);
     if (0.5 < lum)
	 {
        FragColor = vec4(1, 1, 1, 1);
    }
	else 
	{
       FragColor = vec4(0, 0, 0, col.a);
	}
}

//负色Shader 
void func17()
{
	float T = 1.0;                                  
    vec2 st = TexCoords.st;                        
    vec3 irgb = texture2D(texture1, st).rgb;     
    float a = texture2D(texture1, st).a;        
    vec3 neg = vec3(1., 1., 1.)-irgb;               
    FragColor = vec4(mix(irgb,neg, a), a);  
}

//边缘shader
void func18()
{
	const vec2 texOffset = vec2( 0.005, 0.005);
	const vec4 lumcoeff = vec4(0.299, 0.587, 0.114, 0);

	vec2 tc0 = TexCoords.st + vec2(-texOffset.s, -texOffset.t);
	vec2 tc1 = TexCoords.st + vec2(         0.0, -texOffset.t);
	vec2 tc2 = TexCoords.st + vec2(+texOffset.s, -texOffset.t);
	vec2 tc3 = TexCoords.st + vec2(-texOffset.s,          0.0);
	vec2 tc4 = TexCoords.st + vec2(         0.0,          0.0);
	vec2 tc5 = TexCoords.st + vec2(+texOffset.s,          0.0);
	vec2 tc6 = TexCoords.st + vec2(-texOffset.s, +texOffset.t);
	vec2 tc7 = TexCoords.st + vec2(         0.0, +texOffset.t);
	vec2 tc8 = TexCoords.st + vec2(+texOffset.s, +texOffset.t);
																	
	vec4 col0 = texture2D(texture1, tc0);
	vec4 col1 = texture2D(texture1, tc1);
	vec4 col2 = texture2D(texture1, tc2);
	vec4 col3 = texture2D(texture1, tc3);
	vec4 col4 = texture2D(texture1, tc4);
	vec4 col5 = texture2D(texture1, tc5);
	vec4 col6 = texture2D(texture1, tc6);
	vec4 col7 = texture2D(texture1, tc7);
	vec4 col8 = texture2D(texture1, tc8);
	
	vec4 sum = 8.0 * col4 - (col0 + col1 + col2 + col3 + col5 + col6 + col7 + col8);
	FragColor = vec4(sum.rgb, texture2D(texture1, TexCoords).a);
}

//老照片
void func19()
{
	// vec3( 0.299, 0.587, 0.114 ) 是RGB转YUV的参数值，生成灰色图
	float MixColor = dot(texture2D(texture1, TexCoords).rgb, vec3(0.299, 0.587, 0.114));
	// 使用灰色图进行颜色混合
	vec4 blendColor = vec4( 1.2, 1.0, 0.8, 1.0 ); // 调整这个值以修改最终混合色值
	FragColor = vec4(MixColor * blendColor.r, MixColor * blendColor.g, MixColor * blendColor.b, texture2D(texture1, TexCoords).a);
}


//alpha取反Shader 
void func20()
{
	vec4 col = texture2D(texture1, TexCoords.st);  
    float a = 1.0 - col.a;                                
    FragColor = vec4(col.r*a, col.g*a, col.b*a, a);  
}

//溶解效果
uniform float dissolveFactor;
uniform float edgeWidth;
uniform sampler2D NoiseTexture;

void func21()
{
	vec2 texcoord = TexCoords.xy;

    float noiseValue =  texture2D(NoiseTexture, texcoord).r;      
    vec4 color = texture2D(texture1, texcoord);
    if(noiseValue <= dissolveFactor || color.a < 0.01)
    {
        discard;
    }
        
    float EdgeFactor = clamp((noiseValue - dissolveFactor)/(edgeWidth*dissolveFactor), 0.0, 1.0);

    vec4 edgeColor = vec4(0.9, 0.4, 0.0, 1.0);
    FragColor = color * EdgeFactor + edgeColor * (1.0 - EdgeFactor);
    return;
}

//water效果
uniform sampler2D uvTexture;
vec3 waveNormal(vec2 p) {
    vec3 normal = texture2D(uvTexture, p * 0.25).xyz;
    normal = -1.0 + normal * 2.0; //转到[-1,1]
    return normalize(normal);
}

void func22()
{
	float timeFactor = 0.2;
    float offsetFactor = 0.5;
    float refractionFactor = 0.7;
    
    // simple UV animation
    vec2 coord = TexCoords;
    vec3 normal = waveNormal(coord + vec2(Time * timeFactor, 0));
    
    // simple calculate refraction UV offset
    vec2 p = -1 + 2 * coord;
    vec3 eyePos = vec3(0, 0, 100);
    vec3 inVec = normalize(vec3(p, 0) - eyePos);
    vec3 refractVec = refract(inVec, normal, refractionFactor);
    coord += refractVec.xy * offsetFactor;

    FragColor = texture2D(texture1, coord);

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
	//func14();
	//func15();
	//func16();
	//func17();
	func18();
	//func19();
	//func20();
	//func21();
	//func22();

}