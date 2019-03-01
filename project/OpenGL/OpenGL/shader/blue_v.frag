#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec2 v_position[15];

uniform sampler2D texture1; //亮度纹理


void main()
{
	vec4 sum = vec4(0.0);

    sum += texture2D(texture1, v_position[0]) * 0.10315261887346;
    sum += texture2D(texture1, v_position[1]) * 0.099978946435425;
    sum += texture2D(texture1, v_position[2]) * 0.099978946435425;
    sum += texture2D(texture1, v_position[3]) * 0.091031866649314;
    sum += texture2D(texture1, v_position[4]) *  0.091031866649314;
    sum += texture2D(texture1, v_position[5]) * 0.077863681774563;
    sum += texture2D(texture1, v_position[6]) * 0.077863681774563;
    sum += texture2D(texture1, v_position[7]) * 0.062565225976403;
    sum += texture2D(texture1, v_position[8]) * 0.12098536329147;
    sum += texture2D(texture1, v_position[9]) * 0.091324543473425;
    sum += texture2D(texture1, v_position[10]) * 0.091324543473425;
    sum += texture2D(texture1, v_position[11]) * 0.064758798385279;
    sum += texture2D(texture1, v_position[12]) * 0.064758798385279;
    sum += texture2D(texture1, v_position[13]) * 0.043138659781189;
    sum += texture2D(texture1, v_position[14]) * 0.043138659781189;

    FragColor = sum; 
    
}