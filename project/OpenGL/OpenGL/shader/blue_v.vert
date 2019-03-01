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
    
    v_position[0] = vec2(TexCoords.x, TexCoords.y + 0.001171875);
    v_position[1] = vec2(TexCoords.x, TexCoords.y - 0.001171875);
    v_position[2] = vec2(TexCoords.x, TexCoords.y + 0.002734375);
    v_position[3] = vec2(TexCoords.x, TexCoords.y - 0.002734375);
    v_position[4] = vec2(TexCoords.x, TexCoords.y + 0.004296875);
    v_position[5] = vec2(TexCoords.x, TexCoords.y - 0.004296875);
    v_position[6] = vec2(TexCoords.x, TexCoords.y + 0.005859375);
    v_position[7] = vec2(TexCoords.x, TexCoords.y - 0.005859375);
    v_position[8] = vec2(TexCoords.x, TexCoords.y + 0.007421875);
    v_position[9] = vec2(TexCoords.x, TexCoords.y - 0.007421875);
    v_position[10] = vec2(TexCoords.x, TexCoords.y + 0.008984375);
    v_position[11] = vec2(TexCoords.x, TexCoords.y - 0.008984375);
    v_position[12] = vec2(TexCoords.x, TexCoords.y + 0.010546875);
    v_position[13] = vec2(TexCoords.x, TexCoords.y - 0.010546875);
    v_position[14] = vec2(TexCoords.x, TexCoords.y + 0.012109375);
}