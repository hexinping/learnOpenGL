#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 camPos;

uniform vec3  albedo;        //反射率==>类似于漫反射
uniform float metallic;      //金属度
uniform float roughness;     //粗糙度
uniform float ao;            //环境遮蔽因子

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];


const float PI = 3.14159265359;
// ----------------------------------------------------------------------------

//正态分布函数：估算在受到表面粗糙度的影响下，取向方向与中间向量一致的微平面的数量 ==》微平面的取向方向与中间向量的方向越是一致，镜面反射的效果就越是强烈越是锐利
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}

// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

// ----------------------------------------------------------------------------
//几何函数：描述了微平面自成阴影的属性。当一个平面相对比较粗糙的时候，平面表面上的微平面有可能挡住其他的微平面从而减少表面所反射的光线。
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
//菲涅尔方程：描述的是在不同的表面角下表面所反射的光线所占的比率
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec3 N = normalize(Normal); 
    vec3 V = normalize(camPos - WorldPos);  //视觉方向向量 ViewDir
    
	//计算垂直入射的反射率 
	//如果是金属,使用反照率颜色作为F0   如果是非金属，使用插值计算
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	//总共4个点光源来直接表示场景的辐照度
	//为了满足反射率方程，我们循环遍历每一个光源，计算他们独立的辐射率然后求和，接着根据BRDF和光源的入射角来缩放该辐射率
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) 
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);										//LightDir
		vec3 H = normalize(V + L);																//半程向量方向

		float distance    = length(lightPositions[i] - WorldPos);								//光源与片段的距离

		//由于我们线性空间内计算光照（我们会在着色器的尾部进行Gamma校正），我们使用在物理上更为准确的平方倒数作为衰减。************线性空间计算对PBR来说很重要
		float attenuation = 1.0 / (distance * distance);    
		                                    
		vec3 radiance     = lightColors[i] * attenuation;

		//******************计算完整的 Cook-Torrance specular BRDF项
		float NDF = DistributionGGX(N, H, roughness);                    //正态分布函数
        float G   = GeometrySmith(N, V, L, roughness);                   //几何函数
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);      //菲涅尔方程

		vec3 nominator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0

		//******************计算每个光源在反射率方程中的贡献值了
		//kS等于菲涅耳：kS表示光能中被反射的能量的比例
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;	//漫反射光和镜面光不能超过1.0  折射光量比例(kD)应该等于1.0 - kS。
		kD *= 1.0 - metallic;		//因为金属不会折射光线，因此不会有漫反射。所以如果表面是金属的，我们会把系数kD变为0

		// NdotL进行缩放
        float NdotL = max(dot(N, L), 0.0);

		//计算出射光线的值==出射光线的辐射率
		//实际上是反射率方程的在半球领域Ω的积分的结果。
		//但是我们实际上不需要去求积，因为对于所有可能的入射光线方向我们知道只有4个方向的入射光线会影响片段(像素)的着色。因为这样，我们可以直接循环N次计算这些入射光线的方向(N也就是场景中光源的数目)。
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
	}


	//加一个环境光照项给Lo，然后我们就拥有了片段的最后颜色：
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

	//在线性空间中计算光照是非常重要的，因为PBR要求所有输入都是线性的，如果不是这样，我们就会得到不正常的光照。
	//另外，我们希望所有光照的输入都尽可能的接近他们在物理上的取值，这样他们的反射率或者说颜色值就会在色谱上有比较大的变化空间。
	//Lo作为结果可能会变大得很快(超过1)，但是因为默认的LDR输入而取值被截断。所以在伽马矫正之前我们采用色调映射使Lo从LDR的值映射为HDR的值。 ==>帧缓冲需要使用浮点数据格式存储 RGBA16F  RGBA32F..

    // HDR tonemapping 色调映射
    color = color / (color + vec3(1.0));

	//直到现在，我们假设的所有计算都在线性的颜色空间中进行的，因此我们需要在着色器最后
    // gamma correct Gamma校正：因为前面计算衰减的时候用距离的平方作为倒数
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(ambient, 1.0);




}