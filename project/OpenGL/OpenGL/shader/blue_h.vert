#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec2 v_position[15];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos.x, -aPos.y, aPos.z, 1.0);

    v_position[0] = vec2(TexCoords.x + 0.00054112554112554, TexCoords.y);
    v_position[1] = vec2(TexCoords.x - 0.00054112554112554, TexCoords.y);
    v_position[2] = vec2(TexCoords.x + 0.0012626262626263, TexCoords.y);
    v_position[3] = vec2(TexCoords.x - 0.0012626262626263, TexCoords.y);
    v_position[4] = vec2(TexCoords.x + 0.001984126984127, TexCoords.y);
    v_position[5] = vec2(TexCoords.x - 0.001984126984127, TexCoords.y);
    v_position[6] = vec2(TexCoords.x + 0.0027056277056277, TexCoords.y);
    v_position[7] = vec2(TexCoords.x - 0.0027056277056277, TexCoords.y);
    v_position[8] = vec2(TexCoords.x + 0.0034271284271284, TexCoords.y);
    v_position[9] = vec2(TexCoords.x - 0.0034271284271284, TexCoords.y);
    v_position[10] = vec2(TexCoords.x + 0.0041486291486291, TexCoords.y);
    v_position[11] = vec2(TexCoords.x - 0.0041486291486291, TexCoords.y);
    v_position[12] = vec2(TexCoords.x + 0.0048701298701299, TexCoords.y);
    v_position[13] = vec2(TexCoords.x - 0.0048701298701299, TexCoords.y);
    v_position[14] = vec2(TexCoords.x + 0.0055916305916306, TexCoords.y);


}