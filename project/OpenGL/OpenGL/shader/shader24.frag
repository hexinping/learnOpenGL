#version 330 core
out vec4 FragColor;  

in vec2 TexCoords;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

//uniform float textureAlpha;


uniform vec3 objectColor; //物体的颜色
uniform vec3 lightColor; //光的颜色
uniform vec3 lightPos;  //光源的位置
uniform vec3 viewPos;  // 摄像机的位置

in vec3 Normal;
in vec3 FragPos;

//我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色

struct Material {
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    float shininess;

	sampler2D diffuse; //漫反射贴图
	sampler2D specular; //高光贴图
	sampler2D emission; //自发光贴图
}; 

uniform Material material;

struct Light{
    vec3 position;   //光源的位置

	//光源对它的ambient、diffuse和specular光照有着不同的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform samplerCube skybox;



 const float offset = 1.0 / 100.0;  
 uniform vec2 resolution;			//屏幕大小
 uniform vec2 textureSize;			//纹理大小

//函数声明
vec4 blur(vec2 p);
vec4 blur22(vec2 p);
int getIsStrokeWithAngel(float angel,float outlineSize);

vec3 testFunc(vec3 result);
vec3 func1(vec3 result);
vec3 func2(vec3 result);
vec3 func3(vec3 result);
vec3 func4(vec3 result);
vec3 func5(vec3 result);
vec3 func5_1(vec3 result);
vec3 func6(vec3 result);
vec3 func7(vec3 result);
vec3 func7_1(vec3 result);
vec3 func23(vec3 result);


vec3 testFunc(vec3 result)
{
	vec3 color;
	//color = func1(result);
	//color = func2(result);
	//color =func3(result);
	//color =func4(result);
	//color =func5(result);
	//color =func5_1(result);
	//color =func6(result);
	//color =func7(result);
	color =func7_1(result);


	//color =func23(result);

	return color;

}


void main()
{
	//float ambientStrength = 0.1; 

    //vec3 ambient = light.ambient * material.ambient ; //环境光光照

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色

	diffuseMapColor = testFunc(diffuseMapColor);  //觉得在这里操作才是对的

	vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
	vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

	vec3 ambient = light.ambient * diffuseMapColor; //环境光光照


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // 这里为什么不是FragPos - lightPos : 因为法线的方向时垂直平面的，然后lightPos-FragPos构成的向量才能计算dot夹角值

	//计算漫反射光照
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * material.diffuse * light.diffuse; //漫反射光光照
	vec3 diffuse = diff * light.diffuse * diffuseMapColor; //漫反射光光照

	//float specularStrength = 0.5; // 高光强度
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，

	//计算高光光照
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度
    //vec3 specular = spec * material.specular * light.specular;
	vec3 specular = spec * light.specular * specularMapColor;

	

	//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//使用自发光贴图
	vec3 result = (ambient + diffuse + specular) * lightColor * objectColor;
	FragColor = vec4(result , 1.0);

	//添加反射效果
	//vec3 R = reflect(-viewDir, norm);
	//vec3 reflectColor = texture(skybox, R).rgb;

	//FragColor = vec4(result * reflectColor , 1.0);

	//折射
	//float ratio = 1.00 / 1.52; //折射率
    //vec3 I = normalize(Position - cameraPos);
    //vec3 R = refract(I, normalize(Normal), ratio);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

	//vec3 R = refract(-viewDir, norm, ratio);
	//vec3 reflectColor = texture(skybox, R).rgb;
	//FragColor = vec4(reflectColor , 1.0);

	
	//不同shader方法
	//testFunc(result);
	

}

//反色
vec3 func1(vec3 result)
{
	vec3 color = 1.0 - result;
	return color;
}


//灰度1
vec3 func2(vec3 result)
{
	//像素颜色rgb值，取中间值
	float average = (result.r + result.g + result.b) / 3.0;
	vec3 color = vec3(average, average, average);
	return color;
}

//灰度2
vec3 func3(vec3 result)
{
	float average = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
	vec3 color = vec3(average, average, average);
	return color;

}

//核效果
vec3 func4(vec3 result)
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
	    sampleTex[i] = vec3(texture(material.diffuse, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];
	
	
	return col;

}

//模糊：原理就是用该像素周边的像素与自己做平均值插值颜色
vec3 func5(vec3 result)
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
	    sampleTex[i] = vec3(texture(material.diffuse, TexCoords.st + offsets[i])); //取到周边像素的纹理颜色
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i]; //做平均值计算

	return col;

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
            col += texture2D(material.diffuse, p + vec2(x * unit.x *1.5, y * unit.y * 1.5)) * weight; //去周边的像素*权重 计算最后的颜色值
            count += weight;
        }
    }
        
    return col / count;
}

//模糊2
vec3 func5_1(vec3 result)
{
	vec4 col = blur(TexCoords);
	return col.rgb;
}


 vec4 blur22(vec2 p)
 {
    float blurRadius = 4.0;
    float sampleNum = 4.0;
    if (blurRadius > 0.0 && sampleNum > 1.0)
    {
        vec4 col = vec4(0);
        vec2 unit = 1.0 / resolution.xy;
        
        float r = blurRadius;
        float sampleStep = r / sampleNum;
        
        float count = 0.0;
        
        for(float x = -r; x < r; x += sampleStep)
        {
            for(float y = -r; y < r; y += sampleStep)
            {
                float weight = (r - abs(x)) * (r - abs(y));
                col += texture2D(material.diffuse, p + vec2(x * unit.x, y * unit.y)) * weight;
                count += weight;
            }
        }
        
        return col / count;
    }
    return texture2D(material.diffuse, p);
  }

//模糊
vec3 func23(vec3 result)
{
	vec4 col = blur22(TexCoords);
	return col.rgb;
}


//边缘检测
vec3 func6(vec3 result)
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
	    sampleTex[i] = vec3(texture(material.diffuse, TexCoords.st + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++)
	    col += sampleTex[i] * kernel[i];

	return col;

}

//描边1
vec3 func7(vec3 result)
{
	
    float radius = 0.01;
    vec4 accum = vec4(0.0);
    vec4 normal = vec4(0.0);
    
    normal = texture2D(material.diffuse, vec2(TexCoords.x, TexCoords.y)); //取到纹理的颜色
    
	// 取四周纹理的颜色进行均值操作
    accum += texture2D(material.diffuse, vec2(TexCoords.x - radius, TexCoords.y - radius));
    accum += texture2D(material.diffuse, vec2(TexCoords.x + radius, TexCoords.y - radius));
    accum += texture2D(material.diffuse, vec2(TexCoords.x + radius, TexCoords.y + radius));
    accum += texture2D(material.diffuse, vec2(TexCoords.x - radius, TexCoords.y + radius));
    
    accum *= 1.75;

	vec3 outLineColor = vec3(1.0f, 0.2f, 0.3f);
    accum.rgb =  outLineColor * accum.a;
    accum.a = 1.0;
    
    vec4 color = ( accum * (1.0 - normal.a)) + (normal * normal.a);  //根据当前像素的透明度来决定最后插值颜色
    return color.rgb;
}


// 判断在这个角度上距离为outlineSize那一点是不是透明  
int getIsStrokeWithAngel(float angel,float outlineSize)  
{  
    int stroke = 0;  
    float rad = angel * 0.01745329252; // 这个浮点数是 pi / 180，角度转弧度  

	// 这句比较难懂，outlineSize * cos(rad)可以理解为在x轴上投影，除以textureSize.x是因为texture2D接收的是一个0~1的纹理坐标，而不是像素坐标  
    float a = texture2D(material.diffuse, vec2(TexCoords.x + outlineSize * cos(rad) / textureSize.x, TexCoords.y + outlineSize * sin(rad) / textureSize.y)).a; 
    if (a >= 0.8)// 我把alpha值大于0.8都视为不透明，小于0.5都视为透明  
    {  
        stroke = 1;  
    }  
    return stroke;  
}  
  

//描边2： 1. 如果它是不透明的像素，则不管，维持原本颜色；2. 如果透明，是360度判断它四周有没有不透明的像素，如果有，则把它设成描边颜色，否则保持透明。
vec3 func7_1(vec3 result)
{
	
	vec4 color = texture2D(material.diffuse, TexCoords);

	if (color.a >= 0.8) // 不透明，不管，直接返回  
    {  
    	return color.rgb;
    }  

	vec3 outlineColor = vec3(1.0,0,0);				// 描边颜色  
	float outlineSize = 50.0;						// 描边宽度，以像素为单位

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
  
    return color.rgb;

}






