#version 330 core
out vec4 FragColor;  

in vec2 TexCoords;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

//uniform float textureAlpha;


uniform vec3 objectColor; //�������ɫ
uniform vec3 lightColor; //�����ɫ
uniform vec3 lightPos;  //��Դ��λ��
uniform vec3 viewPos;  // �������λ��

in vec3 Normal;
in vec3 FragPos;

//����Ҳ�Ƴ��˻����������ɫ��������Ϊ��������ɫ�ڼ�����������¶�������������ɫ

struct Material {
    //vec3 ambient;
    //vec3 diffuse;
    //vec3 specular;
    float shininess;

	sampler2D diffuse; //��������ͼ
	sampler2D specular; //�߹���ͼ
	sampler2D emission; //�Է�����ͼ
}; 

uniform Material material;

struct Light{
    vec3 position;   //��Դ��λ��

	//��Դ������ambient��diffuse��specular�������Ų�ͬ��ǿ��
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform samplerCube skybox;


//��������
void func1(vec3 result, vec3 diffuseMapColor);



void main()
{
	//float ambientStrength = 0.1; 

    //vec3 ambient = light.ambient * material.ambient ; //���������

	vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //��������ͼ������ɫ
	vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //�߹���ͼ������ɫ
	vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //�Է�����ͼ������ɫ

	vec3 ambient = light.ambient * diffuseMapColor; //���������


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // ����Ϊʲô����FragPos - lightPos : ��Ϊ���ߵķ���ʱ��ֱƽ��ģ�Ȼ��lightPos-FragPos���ɵ��������ܼ���dot�н�ֵ

	//�������������
	float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = diff * material.diffuse * light.diffuse; //����������
	vec3 diffuse = diff * light.diffuse * diffuseMapColor; //����������

	//float specularStrength = 0.5; // �߹�ǿ��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴��

	//����߹����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//�����
    //vec3 specular = spec * material.specular * light.specular;
	vec3 specular = spec * light.specular * specularMapColor;

	

	//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//ʹ���Է�����ͼ
	vec3 result = (ambient + diffuse + specular) * lightColor * objectColor;
	//FragColor = vec4(result , 1.0);

	//��ӷ���Ч��
	//vec3 R = reflect(-viewDir, norm);
	//vec3 reflectColor = texture(skybox, R).rgb;

	//FragColor = vec4(result * reflectColor , 1.0);

	//����
	//float ratio = 1.00 / 1.52; //������
    //vec3 I = normalize(Position - cameraPos);
    //vec3 R = refract(I, normalize(Normal), ratio);
    //FragColor = vec4(texture(skybox, R).rgb, 1.0);

	//vec3 R = refract(-viewDir, norm, ratio);
	//vec3 reflectColor = texture(skybox, R).rgb;
	//FragColor = vec4(reflectColor , 1.0);


	func1(result, diffuseMapColor);

}


//��ɫ
void func1(vec3 result, vec3 diffuseMapColor)
{
	vec3 color = 1.0 -diffuseMapColor;
	result = result + color;
	FragColor = vec4(result , 1.0);
}
