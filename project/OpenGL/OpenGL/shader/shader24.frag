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


//函数声明
void func1(vec3 result, vec3 diffuseMapColor);



void main()
{
	//float ambientStrength = 0.1; 

    //vec3 ambient = light.ambient * material.ambient ; //环境光光照

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
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
	//FragColor = vec4(result , 1.0);

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


	func1(result, diffuseMapColor);

}


//反色
void func1(vec3 result, vec3 diffuseMapColor)
{
	vec3 color = 1.0 -diffuseMapColor;
	result = result + color;
	FragColor = vec4(result , 1.0);
}
