#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);

    vec4 colorMask = texture(ourTexture2, TexCoord);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1g) Vignete

    //color.r = colorMask.r * colorMask.a + color.r * (1.0 - colorMask.a);
    //color.g = colorMask.g * colorMask.a + color.g * (1.0 - colorMask.a);
    //color.b = colorMask.b * colorMask.a + color.b * (1.0 - colorMask.a);
    //color.a = 1.0;

    color = mix(color, colorMask, colorMask.a);
    color = vec4(color.rgb, 1.0);

}