#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;


const int camada3		= 0;
const int camada2		= 1;
const int camada1		= 2;
const int chao			= 3;
const int personagem	= 4;
const int foice			= 5;


//Variável estática para controle da movimentação do personagem
enum Movimento{esquerda,parado,direita};
static Movimento mov=parado;



void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Tela Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}
	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

	glEnable(GL_DEPTH);
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}

//Mudança da variável estática que controla o movimento do personagem de acordo com input de teclas
void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mov = direita;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mov = esquerda;
	}
	else
	{
		mov = parado;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
	//AQUI APLICAREMOS TRANSFORMAÇÕES NOS SPRITES//
	
	//Controle de movimentação do personagem
	switch (mov)
	{
	case esquerda:

		camadas[personagem].objects[0]->setPosX(camadas[personagem].objects[0]->getPosX() - camadas[personagem].getDesloc());
		break;
	case direita:
		camadas[personagem].objects[0]->setPosX(camadas[personagem].objects[0]->getPosX() + camadas[personagem].getDesloc());
		break;
	default:
		break;
	}
	camadas[personagem].objects[0]->setPosition(glm::vec3(camadas[personagem].objects[0]->getPosX(), camadas[personagem].objects[0]->getPosYInicial(), camadas[personagem].objects[0]->getPosZInicial()));
	
	for (int i = 0; i < 4; i++)
	{
		//Movimentação constante das camadas do parallax
		camadas[i].objects[0]->setPosition(glm::vec3(camadas[i].objects[0]->getPosXInicial() + camadas[i].objects[0]->tempo * camadas[i].getDesloc(), camadas[i].objects[0]->getPosYInicial(), camadas[i].objects[0]->getPosZInicial()));
		camadas[i].objects[1]->setPosition(glm::vec3(camadas[i].objects[1]->getPosXInicial() + camadas[i].objects[1]->tempo * camadas[i].getDesloc(), camadas[i].objects[1]->getPosYInicial(), camadas[i].objects[1]->getPosZInicial()));
		//Adição na variação de tempo dentro da variável que todos objetos possuem
		camadas[i].objects[0]->tempo += 0.001f;
		camadas[i].objects[1]->tempo +=0.001f;
		
		//Atualização da posição dos sprites que formam o parallax
		if (camadas[i].objects[0]->getPosX()<=-400.0f)
		{
			
			camadas[i].objects[0]->setPosXInicial(1190.0f);
			camadas[i].objects[0]->tempo=0;
			
		}
		if (camadas[i].objects[1]->getPosX() <= -400.0f)
		{
			camadas[i].objects[1]->tempo = 0;
			camadas[i].objects[1]->setPosXInicial(1190.0f);
		}
	}

	//Colisão do personagem com o objeto hard coded
	if ((camadas[personagem].objects[0]->getPosX()+31 >= camadas[foice].objects[0]->getPosX() - 32.5f) && (camadas[personagem].objects[0]->getPosX() < camadas[foice].objects[0]->getPosX()))
	{
		camadas[foice].objects[0]->setPosX(1200.0f);

	}
	else if ((camadas[personagem].objects[0]->getPosX()-31 <= camadas[foice].objects[0]->getPosX() + 32.5f )&&(camadas[personagem].objects[0]->getPosX()> camadas[foice].objects[0]->getPosX()))
	{
		camadas[foice].objects[0]->setPosX(1200.0f);
	}
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.89f, 0.89f, 0.87f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites

	for (int i = 0; i < 6; i++)
	{
		camadas[i].updateCamada();
	}
	
}

void SceneManager::run()
{

	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	
	//Definindo posições de cada Sprites.
	camadas[camada3].adcionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada3].adcionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada3].setDesloc(-35.0f);//Direção de deslocamento.

	camadas[camada2].adcionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada2].adcionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada2].setDesloc(-50.0f);

	camadas[camada1].adcionarObjeto(402.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada1].adcionarObjeto(1200.0f, 340.0f, 0.0, 800.0f, 550.0f, 0.0f, shader);
	camadas[camada1].setDesloc(-65.0f);
	
	camadas[chao].adcionarObjeto(405.0f, 36.0f, 0.0, 800.0f, 95.0f, 1.0f, shader);
	camadas[chao].adcionarObjeto(1200.0f, 36.0f, 0.0, 800.0f, 95.0f, 1.0f, shader);
	camadas[chao].setDesloc(-65.0f);

	camadas[personagem].adcionarObjeto(50.0f, 113.0f, 0.0f, 60.0f, 120.0f, 0.0f, shader);
	camadas[personagem].setDesloc(0.1f);

	camadas[foice].adcionarObjeto(500.0f, 113.0f, 0.0f, 33.0f, 33.25f, 1.0f, shader);
	
	
	//Adicionando Textura para cada Sprites
	unsigned int texID = loadTexture("../textures/camada3.png");
	camadas[camada3].objects[0]->setTexture(texID);
	camadas[camada3].objects[1]->setTexture(texID);
	
	texID = loadTexture("../textures/camada2.png");
	camadas[camada2].objects[0]->setTexture(texID);
	camadas[camada2].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/camada1.png");
	camadas[camada1].objects[0]->setTexture(texID);
	camadas[camada1].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/chao.png");
	camadas[chao].objects[0]->setTexture(texID);
	camadas[chao].objects[1]->setTexture(texID);

	texID = loadTexture("../textures/personagem.png");
	camadas[personagem].objects[0]->setTexture(texID);

	texID = loadTexture("../textures/foice.png");
	camadas[foice].objects[0]->setTexture(texID);


	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); 
										  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}


