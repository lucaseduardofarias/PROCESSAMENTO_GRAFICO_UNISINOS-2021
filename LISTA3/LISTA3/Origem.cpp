#include <iostream>
#include <string>
#include <assert.h>

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
void drawCall(GLint rCL, Shader s, GLuint vao);  // Função criada com a draw
                                                 // call do exercício para
                                                 // facilitar reutilização

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

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


    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Compilando e buildando o programa de shader
    Shader shader("../shaders/transformations.vs",
        "../shaders/transformations.frag");

    GLuint VAO = setupGeometry();

    glUseProgram(shader.ID);

   
    GLint rasterCodeLoc = glGetUniformLocation(shader.ID, "rasterCode");

    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model0 = glm::mat4(1);
        glm::mat4 model1 = glm::mat4(1);
        glm::mat4 model2 = glm::mat4(1);
        // Transformações
        model0 = glm::translate(model0, glm::vec3(400.0f, 300.0f, 0.0f));
        model0 = glm::rotate(model0, (GLfloat)glfwGetTime(),
            glm::vec3(0.0f, 0.0f, 1.0f));

        model1 = glm::translate(model1, glm::vec3(400.0f, 300.0f, 0.0f));
        model1 = glm::scale(model1, glm::vec3(2.0f, 2.0f, 1.0f));

        model2 = glm::translate(model2, glm::vec3(400.0f, 300.0f, 0.0f));
        model2 = glm::translate(model2, glm::vec3(200.0f, 150.0f, 0.0f));

        glm::mat4 ortho = glm::mat4(1);
        ;

        // Matriz ortográfica - Câmera 2D
        ortho = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

        // Obtem seus identificadores de localização no shader
        GLint modelLoc = glGetUniformLocation(shader.ID, "model");
        GLint projLoc = glGetUniformLocation(shader.ID, "projection");
        // Passa suas informações efetivamente para o shader
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(ortho));

        glLineWidth(5);
        glPointSize(10);

        // Chamada de desenho - drawcall

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model0));
        // VIEWPORT
        glViewport(200, 300, 400, 300);

        drawCall(rasterCodeLoc, shader, VAO);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));

        glViewport(0, 0, 400, 300);

        drawCall(rasterCodeLoc, shader, VAO);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

        glViewport(400, 0, 400, 300);

        drawCall(rasterCodeLoc, shader, VAO);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
    int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry() {
    GLfloat vertices[] = {
        0.0f,    -150.0f, 0.0f, 1.0, 0.0, 0.0,  // Superior esquerdo
        200.0f,  0.0f,    0.0f, 0.0, 1.0, 0.0,  // Inferior esquerdo
        -200.0f, 0.0f,    0.0f, 0.0, 0.0, 1.0,  // Superior esquerdo

        200.0f,  0.0f,    0.0f, 1.0, 1.0, 0.0,
        0.0f,    150.0f,  0.0f, 1.0, 0.0, 1.0,  // Inferior esquerdo
        -200.0f, 0.0f,    0.0f, 0.0, 1.0, 1.0 };

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

    // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array
    // para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}

void drawCall(GLint rCL, Shader s, GLuint vao) {
    glUniform1i(rCL, 0);  // zero é preenchido
    glUseProgram(s.ID);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform1i(rCL, 1);
    glDrawArrays(GL_LINE_LOOP, 0, 6);
    glBindVertexArray(0);
}