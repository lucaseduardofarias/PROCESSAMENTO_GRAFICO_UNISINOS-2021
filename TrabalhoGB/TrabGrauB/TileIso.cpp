#include "TileIso.h"


TileIso::TileIso()
{
	
}

TileIso::TileIso(int _idTile, float _altura, float _largura)
{
	initialize(_idTile, _altura, _largura);
}

void TileIso::initialize(int _idTile,float altura, float largura)
{
	this->IDTile = _idTile;
	this->alturaTile = altura;
	this->larguraTile = largura;
	
	//trabalharemos apenas com sprites retangulares

	float topo, base;
	float altText = 0.25;

	

	switch (IDTile)
	{
	     case 0:
	     {

			 base = 1.0;
			 topo = 0.0;
	     }
		 break;
		 case 1:
		 {
			 base = 2.0;
			 topo = 1.0;

		 }
		 break;
		 case 2:
		 {
			 base = 3.0;
			 topo = 2.0;

		 }
		 break;
		 case 3:
		 {
			 base = 4.0;
			 topo = 3.0;

		 }
		 break;
		 default:
		 {
			 base = 4.0;
			 topo = 4.0;

		 }
		 break;
	}

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f,1.0- topo * altText, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f,1.0- base * altText, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f,1.0- base * altText, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f,1.0- topo * altText  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	transform = glm::mat4(1); //matriz identidade
	texID = -1; //ainda n�o temos
	shader = NULL; //ainda n�o temos
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	angle = 0.0f;
}

void TileIso::setTexture(int texID)
{
	this->texID = texID;
}

void TileIso::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::rotate(transform, angle, axis);
}

void TileIso::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::translate(transform, displacements);
}

void TileIso::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::scale(transform, scaleFactors);
	scale = scaleFactors;
}

void TileIso::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shader->ID, "ourTexture1"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TileIso::update()
{
	//basicamente pedir para atualizar no shader 
	//a matriz de transforma��o

	setTranslation(pos);
	setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
	setScale(scale, false);

	GLint transformLoc = glGetUniformLocation(shader->ID, "model");
	// Pass them to the shaders
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	
}

float TileIso::getLargura()
{
	return this->larguraTile;
}

float TileIso::getAltura()
{
	return this->alturaTile;
}
