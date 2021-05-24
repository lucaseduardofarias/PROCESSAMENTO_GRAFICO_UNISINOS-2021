#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

//Limiar de binarizacao
uniform int limiar;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1f) Custom

    //Conversao em grayscale
    float media = color.r * 0.2125 + color.g * 0.7154 + color.b * 0.0721;
    color.r = color.g = color.b = media;

    float limiarNorm = limiar / 255.0;


    //Paleta de três cores de acordo com a proximidade das médias do limiar - Menores[Verde], Parecidos[Magenta], Maiores[Bordô]

    if(media < limiarNorm - (30/255.0)) 
    {
        color = vec4(0.0, 0.7, 0.0, color.a);
    }
    else if (media >= limiarNorm - (30/255.0) && media <= limiarNorm + (30/255.0))
    {
        color = vec4(0.5, 0.0, 0.5, color.a);
    }
    else
    {
        color = vec4(0.5, 0.0, 0.0, color.a);
    }
}