#version 330 core
out vec4 FragColor;


uniform vec3 objectColor; //物体的颜色
uniform vec3 lightColor; //光的颜色


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    float shininess;
	sampler2D texture_diffuse; //漫反射贴图
	sampler2D texture_specular; //高光贴图
	sampler2D texture_emission; //自发光贴图
	sampler2D texture_normal;  //法线贴图
}; 

uniform Material material;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;

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

//天空盒纹理
uniform samplerCube skybox;

//法线贴图相关
in mat3 TBN;  //TBN逆矩阵
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

//计算点光源输出函数
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 posTBN)
{
	
	vec3 lightDir = normalize(posTBN - TangentFragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos); //fragPos 为世界空间里的顶点位置
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    

	//attenuation *=0.5;
    // 合并结果
    vec3 ambient  =  vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    ambient  *= attenuation ;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}

void main()
{    
	// 属性
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos); //都转换到切线空间计算

	 // 从法线贴图范围[0,1]获取法线
    vec3 norm = texture(material.texture_normal, TexCoords).rgb;
    // 将法线向量转换为范围[-1,1]，因为存到纹理里坐标范围被转换成[0,1],但是使用时法线向量的范围为[-1,1]，所以需要转换
    norm = normalize(norm * 2.0 - 1.0);  // this normal is in tangent space

    vec3 result;
    //点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		
		vec3 posTBN = TBN * pointLights[i].position;  //转到切线空间
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, posTBN);  
	}
        

    result = result * lightColor * objectColor;


	//添加反射效果
	vec3 R = reflect(-viewDir, norm);
	vec3 reflectColor = texture(skybox, R).rgb;
	FragColor = vec4(result * reflectColor, 1.0);

   //FragColor = vec4(result, 1.0);
}