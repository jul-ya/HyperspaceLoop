#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{             
    vec3 color1 = texture(texture1, TexCoords).rgb;      
    vec3 color2 = texture(texture2, TexCoords).rgb;

    color1 += color2; // additive blending
   
    FragColor = vec4(color1, 1.0f);
}