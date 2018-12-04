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

	//vec3 inVec = normalize(FragPos - eyePos);   //视线方向

    vec3 refractVec = refract(inVec, normal, refractionFactor);  //根据入射向量，法线，折射系数计算折射向量
    vec2 v_texCoordN = TexCoord;
    v_texCoordN += refractVec.xy * offsetFactor;    
    //v_texCoordN.x -= Time*timeFactor *0.6; //移动水面贴图，可选
       
    FragColor = texture(texture0, v_texCoordN);  

}