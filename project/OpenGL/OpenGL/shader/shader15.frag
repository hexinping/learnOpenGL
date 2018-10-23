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

uniform Light light;


void main()
{
	
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

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

		//计算点光源的衰减值
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
		attenuation *= 5;

		//平滑/软化边缘
	    float theta = dot(lightDir, normalize(-light.direction)); 
	    float epsilon = (light.cutOff - light.outerCutOff);
	    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	    diffuse  *= intensity;
	    specular *= intensity;

		//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//使用自发光贴图
		vec3 result = (ambient + diffuse + specular) * attenuation * lightColor * objectColor;
		FragColor = vec4(result, 1.0);
	}
	else
	{
		// 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
	}

}