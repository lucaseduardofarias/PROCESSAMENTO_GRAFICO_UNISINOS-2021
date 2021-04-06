
#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
GLuint setupShader();
int setupGeometryParede();
int setupGeometryTelhado();
int setupGeometryPorta();
int setupGeometryJanela0();
int setupGeometryJanela1();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 460\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 460\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Necessário em alguns casos dependendo da configuração do hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "TELA DO PROJETO", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}


	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO_Parede = setupGeometryParede();
	GLuint VAO_Telhado = setupGeometryTelhado();
	GLuint VAO_Porta = setupGeometryPorta();
	GLuint VAO_Janela0 = setupGeometryJanela0();
	GLuint VAO_Janela1 = setupGeometryJanela1();


	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shaderID);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);


		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Parede);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Telhado);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Telhado);
		glDrawArrays(GL_LINE_LOOP, 0, 3);

		
		glUniform4f(colorLoc, 0.4f, 0.1f, 0.1f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Porta);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Porta);
		glDrawArrays(GL_LINE_STRIP, 0, 4);

		
		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela1);
		glDrawArrays(GL_LINE_STRIP, 0, 5);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_Parede);
	glDeleteVertexArrays(1, &VAO_Telhado);
	glDeleteVertexArrays(1, &VAO_Porta);
	glDeleteVertexArrays(1, &VAO_Janela0);
	glDeleteVertexArrays(1, &VAO_Janela1);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


GLuint setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


int setupGeometryParede()
{

	GLfloat vertices[] = {
		-0.6f, -1.0f, 0.0f, 
		-0.6f,  0.2f, 0.0f, 
		 0.6f,  0.2f, 0.0f , 
		 0.6f,  -1.0f, 0.0f,
	};

	GLuint VBO, VAO_Parede;

	
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glGenVertexArrays(1, &VAO_Parede);

	glBindVertexArray(VAO_Parede);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Parede;
}

int setupGeometryTelhado()
{

	GLfloat vertices[] = {
		-0.6f, 0.2f, 0.0f, 
		 0.0f, 1.0f, 0.0f,  
		 0.6f, 0.2f, 0.0f ,  
	};

	GLuint VBO, VAO_Telhado;


	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Telhado);

	glBindVertexArray(VAO_Telhado);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Telhado;
}

int setupGeometryPorta()
{

	GLfloat vertices[] = {
		-0.1f, -1.0f, 0.0f, 
		-0.1f,  -0.5f, 0.0f, 
		 0.2f,  -0.5f, 0.0f , 
		 0.2f,  -1.0f, 0.0f,
	};

	GLuint VBO, VAO_Porta;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Porta);

	glBindVertexArray(VAO_Porta);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO_Porta;
}

int setupGeometryJanela0()
{

	GLfloat vertices[] = {
		-0.4f, -0.4f, 0.0f, //   
		-0.4f,  -0.0f, 0.0f, //
		 0.0f,  -0.0f, 0.0f , // 
		 0.0f,  -0.4f, 0.0f,
	};

	GLuint VBO, VAO_Janela0;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Janela0);

	glBindVertexArray(VAO_Janela0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	return VAO_Janela0;
}

int setupGeometryJanela1()
{

	GLfloat vertices[] = {
		-0.4f, -0.2f, 0.0f, //   
		 0.0f,  -0.2f, 0.0f, //
		 0.0f,  0.0f, 0.0f , // 
		-0.2f,  0.0f, 0.0f,
		-0.2f,  -0.4f, 0.0f
	};

	GLuint VBO, VAO_Janela1;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Janela1);

	glBindVertexArray(VAO_Janela1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	return VAO_Janela1;
}
