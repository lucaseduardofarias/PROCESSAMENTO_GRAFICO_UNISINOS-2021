#version 450 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

// Texture sampler
uniform sampler2D ourTexture1;

uniform vec3 corColorizadora;

void main()
{
    //O comando texture é da própria GLSL, e busca a cor do fragmento com a coord de 
    //textura fornecida
    color = texture(ourTexture1, TexCoord);

    //LEMBRE-SE: aqui o valor dos canais de cor está NORMALIZADO (entre 0.0 e 1.0)
    
    // 1c) Colorizacao
    color = color + vec4(corColorizadora, 0.0);

}