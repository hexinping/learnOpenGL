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

in float useDepthDebug;

//阴影贴图
uniform sampler2D shadowMap;
in vec4 FragPosLightSpace;


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{

	//首先要检查一个片元是否在阴影中，把光空间片元位置转换为裁切空间的标准化设备坐标。
	//当我们在顶点着色器输出一个裁切空间顶点位置到gl_Position时，OpenGL自动进行一个透视除法，将裁切空间坐标的范围-w到w转为-1到1，
	//这要将x、y、z元素除以向量的w元素来实现。由于裁切空间的FragPosLightSpace并不会通过gl_Position传到像素着色器里，我们必须自己做透视除法：
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; //片元在光空间的-1到1的范围

	if(projCoords.z > 1.0)
     {
		//当一个点比光的远平面还要远时，它的投影坐标的z坐标大于1.0 直接返回没有阴影

		//只有在深度贴图范围以内的被投影的fragment坐标才有阴影，所以任何超出范围的都将会没有阴影
		return 0.0;
	 }

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;   //为了作为从深度贴图中采样的坐标，xy分量也需要变换到[0,1]。
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow

	//阴影偏移 避免在距离光源比较远的情况下，多个片元可能从深度贴图的同一个值中去采样
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 

	 // PCF 避免产生阴影锯齿
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}


void main()
{

	if(useDepthDebug != 0.0)
	{
		//float ambientStrength = 0.1; 

		//vec3 ambient = light.ambient * material.ambient ; //环境光光照

		vec3 diffuseMapColor = vec3(texture(material.diffuse, TexCoords)); //漫反射贴图纹理颜色
		//vec3 specularMapColor = vec3(texture(material.specular, TexCoords)); //高光贴图纹理颜色
		//vec3 emissionMapColor = vec3(texture(material.emission, TexCoords)); //自发光贴图纹理颜色

		vec3 ambient = diffuseMapColor; //环境光光照

		//计算点光源的衰减值
		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos); 

		//计算漫反射光照
		float diff = max(dot(norm, lightDir), 0.0);
		//vec3 diffuse = diff * material.diffuse * light.diffuse; //漫反射光光照
		vec3 diffuse = diff * light.diffuse * diffuseMapColor; //漫反射光光照

		//float specularStrength = 0.5; // 高光强度
		vec3 viewDir = normalize(viewPos - FragPos);

		//计算高光光照

		//1 冯氏光照
		//vec3 reflectDir = reflect(-lightDir, norm); //reflect函数要求第一个向量是从光源指向片段位置的向量，但是lightDir当前正好相反，
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//反光度

		//2 Blinn-Phong（冯氏光照的优化版）：用半程向量和法线向量的点积表示镜面反光
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(halfwayDir, norm), 0.0), material.shininess);
	
	
		vec3 specular = spec * light.specular;

	
		//vec3 result = (ambient + diffuse + specular + emissionMapColor) * lightColor * objectColor;//使用自发光贴图
		//vec3 result = (ambient + diffuse + specular) * attenuation * lightColor * objectColor;


		 // calculate shadow
		float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);                      
		vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation * lightColor * objectColor;

		FragColor = vec4(result, 1.0);
	}
	else
	{
		//由于我们没有颜色缓冲，最后的片元不需要任何处理，所以我们可以简单地使用一个空像素着色器
		//这个空像素着色器什么也不干，运行完后，深度缓冲会被更新。我们可以取消那行的注释，来显式设置深度，
		//但是这个（指注释掉那行之后）是更有效率的，因为底层无论如何都会默认去设置深度缓冲

		// gl_FragDepth = gl_FragCoord.z;
	}
	

}