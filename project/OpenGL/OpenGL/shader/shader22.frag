#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform bool useHDR;
uniform float exposure;

uniform bool useBloom;
uniform sampler2D bloomBlur;

const float offset = 1.0 / 300.0;  

void main()
{
	vec3 col = texture(screenTexture, TexCoords).rgb;
	if(!useHDR)
	{
		
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
   

} 