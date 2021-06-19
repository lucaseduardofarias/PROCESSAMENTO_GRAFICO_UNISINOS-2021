#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "TileIso.h"
#include "Personagem.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#define ROWS 5
#define COLS 5

enum Direcoes{PARADO,NORTE,NORDESTE,LESTE,SUDESTE,SUL,SUDOESTE,OESTE,NOROESTE};

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	
	//GLFW callbacks - PRECISAM SER EST�TICAS
	//Para isso, as vari�veis que modificamos dentro deles
	//tamb�m precisam ser e est�o no in�cio do SceneManager.cpp
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	//M�todos pricipais
	void initialize(GLuint width, GLuint height);
	void run();
	void finish();

	//M�todos chamados no run (ciclo)
	void update();
	void render();

	//M�todos para configura��o e carregamento dos elementos da cena
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene(); //antigo setupGeometry
	void setupCamera2D();
	unsigned int loadTexture(string filename); 
	void do_movement();

private:
	
	//Janela GLFW (por enquanto, assumimos apenas 1 inst�ncia)
	GLFWwindow *window;

	//Programa de shader (por enquanto, assumimos apenas 1 inst�ncia)
	Shader *shader;

	//C�mera 2D - Matriz de proje��o (ortogr�fica) com os limites em x,y
	glm::vec4 ortho2D; //xmin, xmax, ymin, ymax
	glm::mat4 projection;

	//glm::mat4 model;
	//Nossos objetos (sprites) da cena
	int map[ROWS][COLS];
	int mapWalking[ROWS][COLS];

	glm::vec2 posPersonagem;
	int posLinha;
	int posColuna;
	bool ind_vitoria;
	

	//vector <Sprite*> objects;
	vector<Sprite*> telas;
	Sprite* mundo;
	bool vivo;
	Personagem* personagem;
	vector <TileIso*> tileset;

};

