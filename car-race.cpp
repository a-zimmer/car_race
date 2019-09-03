#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include </usr/include/glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"

const GLint WIDTH = 1280, HEIGHT = 768;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;
GLuint colorbuffer;
GLuint vertexbuffer;

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

void configLayout(GLuint vertexbuffer, GLuint colorbuffer){

	//1 é o buffer de vértices (buffer 0 )
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // tamanho do vertice
		GL_FLOAT,           // tipo
		GL_FALSE,           // normalizado?
		0,                  // inicia na posição tal* do buffer
		(void*)0            // array buffer offset
	);

	//2 é o buffer de cores (buffer1)
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // tamanho do vertice
		GL_FLOAT,                         // tipo
		GL_FALSE,                         // normalizado?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
}

void PrintNaTela (GLfloat *vertices, int vertexSize, GLfloat *cores, int colorSize)
	{
		//Triangulo 1
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//color RGB
		glBufferData(GL_ARRAY_BUFFER, colorSize, cores, GL_STATIC_DRAW);
		//vertexs
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexSize, vertices, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, vertexSize/sizeof(GLfloat)); 
		
	}

int main(void)
{
	initWindow();

	GLuint VertexArrayID; //conexão com a placa de video
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );

	//Array que representa as coordenadas x e y de um triangulo
	GLfloat g_vertex_buffer_data_car[72] = {
		    /*Car 1*/
		// Body
		-10.0f,10.0f,//A
		-10.0f,-10.0f,//H
		 0.0f,-10.0f,//I
		 //2
		-10.0f,10.0f,//A
		 0.0f,10.0f,//B	
		 0.0f,-10.0f,//I

		//Perna
		-20.0f,-10.0f,//M
		-20.0f,-20.0f,//J
		 10.0f,-20.0f,//K
		 //2
		-20.0f,-10.0f,//M
		 10.0f,-10.0f,//L
		 10.0f,-20.0f,//K

		 //Braços
		-20.0f,10.0f,//D
		-20.0f,0.0f,//E
		 10.0f,0.0f,//G
		 //2
		-20.0f,10.0f,//D
		 10.0f,10.0f,//F
		 10.0f,0.0f,//G
		 //Head
		-15.0f,10.0f,//A
		-5.0f,25.0f,//C
		 5.0f,10.0f,//B
		 //?
		 -20.0f,-20.0f,//J
		 -5.0f,-30.0f,//N
		 10.0f,-20.0f,//K
	};

	GLfloat g_color_buffer_data_car[72] = { 
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,

		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
	};

	glGenBuffers(1, &vertexbuffer);
	
	glGenBuffers(1, &colorbuffer);


	do{
		// Limpa a Tela
		glClear(GL_COLOR_BUFFER_BIT);

		// Para definir os Shaders
		glUseProgram(programID);

		configLayout(vertexbuffer, colorbuffer);

		PrintNaTela(g_vertex_buffer_data_car,sizeof(g_vertex_buffer_data_car), g_color_buffer_data_car,sizeof(g_color_buffer_data_car));

		glDisableVertexAttribArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
