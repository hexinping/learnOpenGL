#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float BloomThreshold;
uniform sampler2D texture1;


void main()
{
    vec4 color = texture2D(texture1, TexCoords);
    FragColor = clamp((color - BloomThreshold) / (1.0 - BloomThreshold), 0.0, 1.0);
}