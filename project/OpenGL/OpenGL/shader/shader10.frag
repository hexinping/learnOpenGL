#version 330 core
out vec4 FragColor;  

//in vec2 TexCoord;

//uniform sampler2D texture1;
//uniform sampler2D texture2;

//uniform float textureAlpha;


uniform vec3 objectColor; //�������ɫ
uniform vec3 lightColor; //�����ɫ
uniform vec3 lightPos;  //��Դ��λ��
uniform vec3 viewPos;  // �������λ��

in vec3 Normal;
in vec3 FragPos;

void main()
{
	float ambientStrength = 0.1; 
    vec3 ambient = ambientStrength * lightColor; //���������

	//ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ���
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	//FragColor =  vec4(color.xyz * lightColor,1.0f);


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // ����Ϊʲô����FragPos - lightPos 

	//�������������
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor; //����������

	float specularStrength = 0.5; // �߹�ǿ��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); //reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴��

	//����߹����
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);

}