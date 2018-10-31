#version 330 core
out vec4 FragColor;  

in vec2 TexCoords;

uniform vec3 objectColor; //物体的颜色
uniform vec3 lightColor; //光的颜色
uniform vec3 lightPos;  //光源的位置
uniform vec3 viewPos;  // 摄像机的位置

in vec3 Normal;
in vec3 FragPos;

//我们也移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色

struct Material {
    float shininess;
	sampler2D diffuse; //漫反射贴图
	sampler2D specular; //高光贴图
	sampler2D emission; //自发光贴图
}; 

uniform Material material;

// 定向光
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;




//点光源配置
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

//定义聚光配置
struct SpotLight{

    vec3  position;
    vec3  direction;
    float cutOff;  //切光角
    float outerCutOff;

	//光源对它的ambient、diffuse和specular光照有着不同的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // 定向光的衰减变量
    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;


//计算定向光输出函数
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色

	//1 冯氏光照
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess);

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//计算点光源输出函数
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色

	//1 冯氏光照
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess);


    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}

//定义聚光输出
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));//片段指向光源的向量 
	if(theta > light.cutOff)
	{

		//float ambientStrength = 0.1; 
	    //vec3 ambient = light.ambient * material.ambient ; //环境光光照

		vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
		vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
		vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

		vec3 ambient = light.ambient * diffuseMapColor; //环境光光照

		//计算漫反射光照
		float diff = max(dot(normal, lightDir), 0.0);
		//vec3 diffuse = diff * material.diffuse * light.diffuse; //漫反射光光照
		vec3 diffuse = diff * light.diffuse * diffuseMapColor; //漫反射光光照

		//float specularStrength = 0.5; // 高光强度
		vec3 viewDir = normalize(viewPos - FragPos);
		

		//计算高光光照
		//1 冯氏光照
		//vec3 reflectDir = reflect(-lightDir, normal); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度

		//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess);


		vec3 specular = spec * light.specular * specularMapColor;

		
		//计算点光源的衰减值
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		attenuation *= 2;

		//平滑/软化边缘
	    float theta = dot(lightDir, normalize(-light.direction)); 
	    float epsilon = (light.cutOff - light.outerCutOff);
	    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	    diffuse  *= intensity;
	    specular *= intensity;

		return (ambient + diffuse + specular) * attenuation;
	}
}


void main()
{
	
	// 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
   	vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    result = result * lightColor * objectColor;
    FragColor = vec4(result, 1.0);
}

