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
    //vec3 position;   //光源的位置 
    vec3 direction; // 使用定向光就不再需要光源的位置了

	//光源对它的ambient、diffuse和specular光照有着不同的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;


void main()
{
	//float ambientStrength = 0.1; 

    //vec3 ambient = light.ambient * material.ambient ; //环境光光照

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
	vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
	vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

	vec3 ambient = light.ambient * diffuseMapColor; //环境光光照

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);  //定义的光源方向时从光源位置到顶点位置，而计算漫反射需要从顶点到光源的向量，所以用-light.direction来计算漫反射

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
	FragColor = vec4(result, 1.0);

}