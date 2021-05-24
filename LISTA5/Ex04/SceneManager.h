#pragma once

#include "Shader.h"
#include "GameObject.h" 

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void update();
	void render();
	void run();
	void finish();


	// scene
	void setupScene();
	void setupCamera2D();
	unsigned int loadTexture(string filename); 

private:
	
	//GFLW window
	GLFWwindow *window;

	//our shader program
	//Shader *shader;
	vector <Shader*> shaders;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//our game objects
	vector <GameObject*> objects;

};

