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

uniform float far_plane;
uniform bool shadows;

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
    //vec3 direction; // 使用定向光就不再需要光源的位置了

	//光源对它的ambient、diffuse和specular光照有着不同的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 定向光的衰减变量
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;



//阴影贴图
uniform samplerCube depthMap;

uniform float isDisplayDepth;
uniform float isUsePCFShadow;

//pcf 解决阴影锯齿感
//由于万向阴影贴图基于传统阴影映射的原则，它便也继承了由解析度产生的非真实感。如果你放大就会看到锯齿边了。
//PCF或称Percentage-closer filtering允许我们通过对fragment位置周围过滤多个样本，并对结果平均化。

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float ShadowCalculationPCF(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
	float closestDepth = 0.0;
    for(int i = 0; i < samples; ++i)
    {
        closestDepth = texture(depthMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
        
    // display closestDepth as debug (to visualize depth cubemap)
    if(isDisplayDepth == 1.0)
	{
		FragColor = vec4(vec3(closestDepth / far_plane), 1.0); 
	}
    return shadow;    

}



float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;

    // ise the fragment to light vector to sample from the depth map    
    float closestDepth = texture(depthMap, fragToLight).r;

    // it is currently in linear range between [0,1], let's re-transform it back to original depth value
    closestDepth *= far_plane;

    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // test for shadows
    float bias = 0.5; // we use a much larger bias since depth is now in [near_plane, far_plane] range
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;   
	     
    // display closestDepth as debug (to visualize depth cubemap)
	if(isDisplayDepth == 1.0)
	{
		FragColor = vec4(vec3(closestDepth / far_plane), 1.0); 
	}
    return shadow;

}


void main()
{

	//float ambientStrength = 0.1; 

	//vec3 ambient = light.ambient * material.ambient ; //环境光光照

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
	//vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
	//vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

	vec3 ambient = diffuseMapColor; //环境光光照

	//计算点光源的衰减值
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

	//计算漫反射光照
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * material.diffuse * light.diffuse; //漫反射光光照
	vec3 diffuse = diff * light.diffuse * diffuseMapColor; //漫反射光光照

	//float specularStrength = 0.5; // 高光强度
	vec3 viewDir = normalize(viewPos - FragPos);

	//计算高光光照

	//1 冯氏光照
	//vec3 reflectDir = reflect(-lightDir, norm); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度

	//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
	
	
	vec3 specular = spec * light.specular;

	
	//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//使用自发光贴图
	//vec3 result = (ambient + diffuse + specular) * attenuation * lightColor * objectColor;


	// calculate shadow
	if(isDisplayDepth)
	{
		if(isUsePCFShadow)
		{
			ShadowCalculationPCF(FragPos);
		}
		else
		{
			ShadowCalculation(FragPos);
		}
		
	}
	else
	{	
		float shadow = 0.0;
		if(isUsePCFShadow)
		{
			shadow = shadows ? ShadowCalculationPCF(FragPos) : 0.0;  
		}
		else
		{
			shadow = shadows ? ShadowCalculation(FragPos) : 0.0;
		}            
		vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation * lightColor * objectColor;
		FragColor = vec4(result, 1.0);
	}
	
}