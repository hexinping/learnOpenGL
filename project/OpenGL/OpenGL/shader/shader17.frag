#version 330 core
out vec4 FragColor;


uniform vec3 objectColor; //物体的颜色
uniform vec3 lightColor; //光的颜色
uniform vec3 viewPos;  // 摄像机的位置

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    float shininess;
	sampler2D texture_diffuse; //漫反射贴图
	sampler2D texture_specular; //高光贴图
	sampler2D texture_emission; //自发光贴图
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

//计算点光源输出函数
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	
	vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    
    // 合并结果
    vec3 ambient  =  vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}

void main()
{    
	// 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;
    //点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

    result = result * lightColor * objectColor;


	//添加反射效果
	vec3 R = reflect(-viewDir, norm);
	vec3 reflectColor = texture(skybox, R).rgb;
	//FragColor = vec4(result + reflectColor, 1.0);

    FragColor = vec4(result, 1.0);
}