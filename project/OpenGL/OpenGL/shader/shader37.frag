#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool useHDR;
uniform float exposure;

uniform bool useBloom;
uniform sampler2D bloomBlur;


//延迟光照信息
uniform bool useDelayRenderLight; 

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 viewPos;

//点光源配置
struct PointLight {
    vec3 position;
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
	float radius; //光体积半径  光源能够到达片段最大距离
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];


const float offset = 1.0 / 300.0;  

void main()
{
	
	if(!useHDR)
	{
		vec3 col = texture(screenTexture, TexCoords).rgb;
		FragColor = vec4(col, 1.0);

		//反相
		//vec3 col = texture(screenTexture, TexCoords).rgb;
		//FragColor = vec4(1.0-col, 1.0);

		//灰度1
		//FragColor = texture(screenTexture, TexCoords);
		//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
		//FragColor = vec4(average, average, average, 1.0);

		//灰度2
		//FragColor = texture(screenTexture, TexCoords);
		//float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
		//FragColor = vec4(average, average, average, 1.0);


		//核效果
		// vec2 offsets[9] = vec2[](
		//     vec2(-offset,  offset), // 左上
		//     vec2( 0.0f,    offset), // 正上
		//     vec2( offset,  offset), // 右上
		//     vec2(-offset,  0.0f),   // 左
		//     vec2( 0.0f,    0.0f),   // 中
		//     vec2( offset,  0.0f),   // 右
		//     vec2(-offset, -offset), // 左下
		//     vec2( 0.0f,   -offset), // 正下
		//     vec2( offset, -offset)  // 右下
		// );

		// float kernel[9] = float[](
		//     -1, -1, -1,
		//     -1,  9, -1,
		//     -1, -1, -1
		// );

		// vec3 sampleTex[9];
		// for(int i = 0; i < 9; i++)
		// {
		//     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		// }
		// vec3 col = vec3(0.0);
		// for(int i = 0; i < 9; i++)
		//     col += sampleTex[i] * kernel[i];

		// FragColor = vec4(col, 1.0);


		//模糊
		// vec2 offsets[9] = vec2[](
		//     vec2(-offset,  offset), // 左上
		//     vec2( 0.0f,    offset), // 正上
		//     vec2( offset,  offset), // 右上
		//     vec2(-offset,  0.0f),   // 左
		//     vec2( 0.0f,    0.0f),   // 中
		//     vec2( offset,  0.0f),   // 右
		//     vec2(-offset, -offset), // 左下
		//     vec2( 0.0f,   -offset), // 正下
		//     vec2( offset, -offset)  // 右下
		// );

		// float kernel[9] = float[](
		//     1.0 / 16, 2.0 / 16, 1.0 / 16,
		//     2.0 / 16, 4.0 / 16, 2.0 / 16,
		//     1.0 / 16, 2.0 / 16, 1.0 / 16  
		// );

		// vec3 sampleTex[9];
		// for(int i = 0; i < 9; i++)
		// {
		//     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		// }
		// vec3 col = vec3(0.0);
		// for(int i = 0; i < 9; i++)
		//     col += sampleTex[i] * kernel[i];

		// FragColor = vec4(col, 1.0);


		//边缘检测
		// vec2 offsets[9] = vec2[](
		//     vec2(-offset,  offset), // 左上
		//     vec2( 0.0f,    offset), // 正上
		//     vec2( offset,  offset), // 右上
		//     vec2(-offset,  0.0f),   // 左
		//     vec2( 0.0f,    0.0f),   // 中
		//     vec2( offset,  0.0f),   // 右
		//     vec2(-offset, -offset), // 左下
		//     vec2( 0.0f,   -offset), // 正下
		//     vec2( offset, -offset)  // 右下
		// );

		// float kernel[9] = float[](
		//      1, 1, 1,
		//      1, -8, 1,
		//      1, 1, 1
		//  );

		// vec3 sampleTex[9];
		// for(int i = 0; i < 9; i++)
		// {
		//     sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
		// }
		// vec3 col = vec3(0.0);
		// for(int i = 0; i < 9; i++)
		//     col += sampleTex[i] * kernel[i];

		// FragColor = vec4(col, 1.0);
	}
	else
	{
		
		if(!useDelayRenderLight)
		{
			vec3 col = texture(screenTexture, TexCoords).rgb;
			const float gamma = 2.2;
			vec3 hdrColor = col;

			//方法一：Reinhard色调映射，它涉及到分散整个HDR颜色值到LDR颜色值上，所有的值都有对应。
			// Reinhard色调映射
			//vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
			// Gamma校正
			//mapped = pow(mapped, vec3(1.0 / gamma));

			//FragColor = vec4(mapped, 1.0);


			//方法二:曝光(Exposure)色调映射
			// reinhard
			// vec3 result = hdrColor / (hdrColor + vec3(1.0));


			//bloom效果
			if(useBloom)
			{
				vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
				hdrColor += bloomColor; // additive blending
			}
			
			// exposure
			vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
			// also gamma correct while we're at it        暂时不使用gamma校正
			result = pow(result, vec3(1.0 / gamma));
			FragColor = vec4(result, 1.0);
		}
		else
		{
			//延迟光照处理

			vec3 FragPos = texture(gPosition, TexCoords).rgb;
			vec3 Normal = texture(gNormal, TexCoords).rgb;
			vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
			float Specular = texture(gAlbedoSpec, TexCoords).a;

			// then calculate lighting as usual
			vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
			vec3 viewDir  = normalize(viewPos - FragPos);
			for(int i = 0; i < NR_POINT_LIGHTS; ++i)
			{
				//计算光体积 进一步优化
				float distance = length(pointLights[i].position - FragPos);
				//暂时关闭光体积的检测，方便查看效果，实践中需要考虑
				//if(distance < pointLights[i].radius)  
				{
					// diffuse
					vec3 lightDir = normalize(pointLights[i].position - FragPos);
					vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * pointLights[i].color;
					// specular
					vec3 halfwayDir = normalize(lightDir + viewDir);  
					float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
					vec3 specular = pointLights[i].color * spec * Specular;
					// attenuation
					float distance = length(pointLights[i].position - FragPos);
					float attenuation = 1.0 / (1.0 + pointLights[i].linear * distance + pointLights[i].quadratic * distance * distance);
					diffuse *= attenuation;
					specular *= attenuation;

					lighting += diffuse + specular;  
				}
				
				      
			}
			FragColor = vec4(lighting, 1.0);

		}
		
	}
   

} 