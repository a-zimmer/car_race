#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "common/shader.hpp"

const GLint WIDTH = 1280, HEIGHT = 768;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;

int initWindow ()
{
	if(!glfwInit()) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}

	// Aplica o contexto atual da máquina de estados para Janela Atual
	glfwMakeContextCurrent(window);

	// Necessário para core profile
	glewExperimental = true;

	//deve ativar antes da chamada glewInit()
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(R, G, B, A);
}

void destroyWindows (GLuint vertexbuffer, GLuint VertexArrayID, GLuint programID)
{
	// Cleanup VBO and VAO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glfwTerminate();
}

int main(void)
{
	initWindow();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//Leitura e compilação dos Shaders em tempo de execução
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	//Array que representa as coordenadas x e y de um triangulo
	static const GLfloat vertex_buffer_data_triangle[] = {
		// Body
		-2.0f,0.0f,//A
		-2.0f,-4.0f,//C
		 0.0f,-4.0f,//D
		 //2
		-2.0f,0.0f,//A
		0.0f,0.0f, //B	
		0.0f,-4.0f,//D
	};

	//Gerar 1 buffer, colocar o identificador resultante em vertexbuffer
	GLuint vertexbuffer;

	//Gerar 1 buffer, colocar o identificador resultante em vertexbuffer
	glGenBuffers(1, &vertexbuffer);

	//Isso identificará nosso buffer de vértice
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	//envia vértices a Opengl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_triangle), vertex_buffer_data_triangle, GL_STATIC_DRAW);

	do{
		// Limpa a Tela
		glClear(GL_COLOR_BUFFER_BIT);

		// Para definir os Shaders
		glUseProgram(programID);

		// Primeiro buffer de atributo: vértices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // Atributo 0. Nenhum motivo específico para 0, mas deve corresponder ao layout no shader.
			2,                  // Especifica o número de componentes por atributo vertex. Deve ser 1, 2, 3 ou 4. O valor inicial é 4.
			GL_FLOAT,           // tipo do dado
			GL_FALSE,           // normalizado?
			0,                  // inicio
			(void*)0            // array buffer offset
		);

		// Desenha o triangulo
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertex_buffer_data_triangle)); // 3 indices iniciando de 0 e 72 bytes

		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
