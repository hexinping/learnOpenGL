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
	sampler2D normal;
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

//in mat3 TBN;  TBN逆矩阵

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;


void main()
{
	//float ambientStrength = 0.1; 

    //vec3 ambient = light.ambient * material.ambient ; //环境光光照

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
	//vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
	//vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

	 // 从法线贴图范围[0,1]获取法线
    vec3 normal = texture(material.normal, TexCoords).rgb;
    // 将法线向量转换为范围[-1,1]，因为存到纹理里坐标范围被转换成[0,1],但是使用时法线向量的范围为[-1,1]，所以需要转换
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

	

	vec3 ambient = diffuseMapColor; //环境光光照

	//计算点光源的衰减值
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


	//vec3 norm = normalize(Normal); //使用法线矩阵  
	vec3 norm = normal; //使用法线贴图   法线原本就已经在切线空间
	//vec3 lightDir = TBN * normalize(lightPos - FragPos);  //乘以逆矩阵 转到切线空间

	vec3 lightDir =  normalize(TangentLightPos - TangentFragPos);  //已经在切线空间

	//计算漫反射光照
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * material.diffuse * light.diffuse; //漫反射光光照
	vec3 diffuse = diff * light.diffuse * diffuseMapColor; //漫反射光光照

	//float specularStrength = 0.5; // 高光强度
	//vec3 viewDir = TBN * normalize(viewPos - FragPos); //乘以逆矩阵 转到切线空间

	vec3 viewDir = normalize(TangentViewPos - TangentFragPos); //已经在切线空间

	//计算高光光照

	//1 冯氏光照
	//vec3 reflectDir = reflect(-lightDir, norm); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度

	//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
	
	
	vec3 specular = spec * light.specular;

	
	//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//使用自发光贴图
	vec3 result = (ambient + diffuse + specular) * attenuation * lightColor * objectColor;
	FragColor = vec4(result, 1.0);

}