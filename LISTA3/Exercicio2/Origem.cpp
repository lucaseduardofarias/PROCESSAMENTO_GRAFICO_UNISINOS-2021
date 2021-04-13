
#include <iostream>
#include <string>
#include <assert.h>
#include <stdio.h> 

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../LISTA3/Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action,
    int mode);

// Protótipos das funções
int setupGeometry();
void drawCall(GLint rCL, Shader s, GLuint vao);  
                                                

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1024, HEIGHT = 768;

enum Movment { Left, Right, Top, Down, Stop };
static Movment mov = Stop;
// Função MAIN
int main() {
    // Inicialização da GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Criação da janela GLFW
    GLFWwindow* window =
        glfwCreateWindow(WIDTH, HEIGHT, "Tela", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros d funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da
    // aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Compilando e buildando o programa de shader
    Shader shader("../shaders/transformations.vs",
        "../shaders/transformations.frag");

    // Gerando um buffer simples, com a geometria de um triângulo
    GLuint VAO = setupGeometry();

    glUseProgram(shader.ID);

    // Para enviar o identificador de modo de desenho
    GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");
    float x, y;
    x = 0.0f;
    y = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e
        // chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);  // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model0 = glm::mat4(1);

        // Transformações
        switch (mov) {
            case Left:

                x -= 0.2f;
                break;
            case Right:

                x += 0.2f;
                break;
            case Top:

                y -= 0.2f;
                break;
            case Down:

                y += 0.2f;
                break;
            default:
                break;
        }
 
     
        model0 = glm::translate(model0, glm::vec3(x, y, 0.0f));
        // Create transformations -- a first "camera" -- Orthographic Camera
        glm::mat4 ortho = glm::mat4(1);
    
        // Matriz ortográfica - Câmera 2D
        ortho = glm::ortho(0.0f, 1024.0f, 768.0f, 0.0f, -1.0f, 1.0f);
        
        // Obtem seus identificadores de localização no shader
        GLint modelLoc = glGetUniformLocation(shader.ID, "model");
        GLint projLoc = glGetUniformLocation(shader.ID, "projection");
        // Passa suas informações efetivamente para o shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model0));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

        glLineWidth(5);
        glPointSize(10);

        // Chamada de desenho - drawcall
        // VIEWPORT
        glViewport(0, 0, 1024, 768);
        drawCall(rasterCodeLoc, shader, VAO);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);
    // Finaliza a execução da GLFW, limpando os recursos alocados por ela

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
   

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mov = Right;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mov = Left;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mov = Top;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mov = Down;
    }
    else {
        mov = Stop;
    }
}

int setupGeometry() {
    GLfloat vertices[] = {
        400.0f, 150.0f, 0.0f, 1.0, 0.0, 0.0,  // Superior esquerdo
        600.0f, 300.0f, 0.0f, 0.0, 1.0, 0.0,  // Inferior esquerdo
        200.0f, 300.0f, 0.0f, 0.0, 0.0, 1.0,  // Superior esquerdo

        600.0f, 300.0f, 0.0f, 1.0, 1.0, 0.0,
        400.0f, 450.0f, 0.0f, 1.0, 0.0, 1.0,  // Inferior esquerdo
        200.0f, 300.0f, 0.0f, 0.0, 1.0, 1.0 };

    GLuint VBO, VAO;

    // Geração do identificador do VBO
    glGenBuffers(1, &VBO);
    // Faz a conexão (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Geração do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        ((GLvoid*)(3 * sizeof(GLfloat))));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

void drawCall(GLint rCL, Shader s, GLuint vao) {
    glUniform1i(rCL, 0);  // zero é preenchido
    glUseProgram(s.ID);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Chamada de desenho - drawcall
    // Contorno do polígono - GL_LINE_LOOP
    glUniform1i(rCL, 1);  // 1 é contornado
    glDrawArrays(GL_LINE_LOOP, 0, 6);
    glBindVertexArray(0);
}