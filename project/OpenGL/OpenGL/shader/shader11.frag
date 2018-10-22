#version 330 core
out vec4 FragColor;  

//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

//uniform float textureAlpha;


uniform vec3 objectColor; //物体的颜色
uniform vec3 lightColor; //光的颜色
uniform vec3 lightPos;  //光源的位置
uniform vec3 viewPos;  // 摄像机的位置

in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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


void main()
{
	//float ambientStrength = 0.1; 

    vec3 ambient = light.ambient * material.ambient * lightColor; //环境光光照

	//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	//FragColor =  vec4(color.xyz * lightColor,1.0f);


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // 这里为什么不是FragPos - lightPos 

	//计算漫反射光照
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (diff * material.diffuse * light.diffuse) * lightColor; //漫反射光光照

	//float specularStrength = 0.5; // 高光强度
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，

	//计算高光光照
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度
    vec3 specular = (spec * material.specular * light.specular) * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);

}