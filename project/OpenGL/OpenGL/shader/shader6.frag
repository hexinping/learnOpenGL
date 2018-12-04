#version 330 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float textureAlpha;
uniform float speedX;
uniform float speedY;

uniform float Time;
uniform float offsetX1;
uniform float offsetY1;

uniform float offsetX2;
uniform float offsetY2;

void main()
{
	//最终输出颜色现在是两个纹理的结合。GLSL内建的mix函数需要接受两个值作为参数，并对它们根据第三个参数进行线性插值。
	//如果第三个值是0.0，它会返回第一个输入；如果是1.0，会返回第二个输入值。
	//0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色

    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);


	//使用外部传来的顶点颜色混合
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	//FragColor = color * vec4(ourColor,1.0);


	//修改纹理texture2的水平翻转
	//vec2 textcoord2 = vec2(-TexCoord.x,TexCoord.y);
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, textcoord2), 0.2);
	//FragColor = color * vec4(ourColor,1.0);

	//使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度
	//vec4 color = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), textureAlpha);
	//FragColor = color * vec4(ourColor,1.0);




	float r = sqrt(TexCoord.x*TexCoord.x + TexCoord.y*TexCoord.y);
    float x = cos(1.0*r + Time * 1.0) /80;
	float y = sin(15.0*r + Time * 3.0) /60;
    FragColor = texture(texture1, TexCoord+ vec2(x,y));

}