#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture_normal;

uniform float Time;
in vec3 FragPos;

vec3 waveNormal(vec2 p) {  
    vec3 normal = texture(texture_normal, p).xyz;  
    normal = -1.0 + normal * 2.0;  
    return normalize(normal);  
}  

//高光计算 参见基本光照公式
//参数说明：n法向量， l入射光向量， e观察向量
float specular(vec3 n,vec3 l,vec3 e, float shininess) 
{    
	vec3 reflectDir = reflect(l, n);
	float spec = pow(max(dot(e, reflectDir), 0.0), shininess);
	return spec;
}


//water效果

void func22()
{
	float timeFactor = 0.2;
    float offsetFactor = 0.5;
    float refractionFactor = 0.7;
    
    // simple UV animation
    vec2 coord = TexCoord;
    vec3 normal = waveNormal(coord + vec2(Time * timeFactor, 0));
    
    // simple calculate refraction UV offset
    vec2 p = -1 + 2 * coord;
    vec3 eyePos = vec3(0, 0, 100);
    vec3 inVec = normalize(vec3(p, 0) - eyePos);
    vec3 refractVec = refract(inVec, normal, refractionFactor);
    coord += refractVec.xy * offsetFactor;

    FragColor = texture2D(texture0, coord);

}

void main()
{

	//原理：让一个像素周期性的运动：找到一个周期往返公式

	//简易谐公式
	//float r = sqrt(TexCoord.x*TexCoord.x + TexCoord.y*TexCoord.y);
    //float x = cos(1.0*r + Time * 1.0) /80;
	//float y = sin(15.0*r + Time * 3.0) /60;
    //FragColor = texture(texture0, TexCoord+ vec2(x,y));


	//带法线模式
	float timeFactor = 0.1;  
    float offsetFactor = 0.5;  
    float refractionFactor = 0.7;  //折射率

	 // simple UV animation   
	 // time/10.0f, time, time*2, time*4
	 vec3 normal = waveNormal(TexCoord + vec2(-Time * timeFactor, Time/10.0f * timeFactor));  



	// simple calculate refraction UV offset  
   
	vec2 p = -1.0 + 2.0 * TexCoord;  
    vec3 eyePos = vec3(0, 0, 10); //眼睛位置 位于中心点正上方  
    vec3 inVec = normalize(vec3(p,0) - eyePos);  //视线方向

	//vec3 inVec = normalize(FragPos - eyePos);   //视线方向反方向

    vec3 refractVec = refract(inVec, normal, refractionFactor);  //根据入射向量，法线，折射系数计算折射向量
    vec2 v_texCoordN = TexCoord;
    v_texCoordN += refractVec.xy * offsetFactor;    
    //v_texCoordN.x -= Time*timeFactor *0.6; //移动水面贴图，可选
       
    //FragColor = texture(texture0, v_texCoordN);  


	//传入变量有问题，实际应用需要修改
	 vec3 light = vec3(0.2,0.1,0);    
    // bteitler: Apply specular highlight
    vec3 normalN = waveNormal(v_texCoordN);
    //vec3 color = vec3(specular(normalN,light,eyePos,3.0));

	//正常传入参数
	vec3 lightDir = normalize(light - FragPos); 
	vec3 viewDir =  normalize(eyePos - FragPos);
    vec3 color = vec3(specular(normalN, -lightDir , viewDir, 1.0));
 
    //gl_FragColor = texture(texture0, v_texCoordN); //这样可以看到不带高光的水面，可选
    //gl_FragColor = texture(u_normalMap, v_texCoord); //这样可以只看法线图，可选
    gl_FragColor = texture(texture0, v_texCoordN)*0.9 + vec4(color,1); //加了高光效果的水面

	//func22();

}