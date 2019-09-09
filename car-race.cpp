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

const GLint WIDTH = 1366, HEIGHT = 768;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;
GLuint colorbuffer;
GLuint vertexbuffer;
double xposMouse, yposMouse;
int widthWindow, heightWindow;

glm::mat3 translation = glm::mat3(1.0f);




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
	//Habilita a captura das teclas retornando GLFW_PRESS pela funcao glfwGetKey()
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//  Mostra o fragmento que estiver mais próximo da câmera Z-Buffer
	glDepthFunc(GL_LESS);
}

void destroyWindows (GLuint vertexbuffer, GLuint VertexArrayID, GLuint programID)
{
	// Cleanup VBO and VAO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glfwTerminate();
}

/*void KeyBoard(GLWindow* window, glm::vec3& positon) {	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position.z -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position.z += 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position.x -= 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position.x += 0.01f;
	}
}*/


void KeyboardMovimentObject(){
	glfwGetCursorPos(window, &xposMouse, &yposMouse);
	glfwGetWindowSize(window,&widthWindow, &heightWindow);
	double horizontal = double(xposMouse * 2 - widthWindow)/double(widthWindow);
	double vertical = double(heightWindow - yposMouse * 2)/double(heightWindow);
	printf("%lf %lf\n",xposMouse,yposMouse);
	printf("%lf %lf\n",horizontal,vertical);
	
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //Up
	{		
		translation[0][2] = horizontal;
		translation[1][2] = vertical;

	}else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //Down
	{		
		translation[0][2] = horizontal;
		translation[1][2] = vertical;

	}else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //Right
	{		
		translation[0][2] = horizontal-0.3;
		translation[1][2] = vertical-0.1;

	}else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //Left
	{		
		translation[0][2] = horizontal+0.3;
		translation[1][2] = vertical-0.1;
	}
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
		//Carro
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
		// Body
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//2
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//Perna
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//2
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//Braços
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//2
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//Head
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		//?
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
	};

	GLfloat g_vertex_buffer_data_pista2[144] = {
		    /*Pista 2*/
		//Asfalto
		 88.0f,-100.0f,//S
		-88.0f,100.0f,//T2
		-88.0f,-100.0f,//A2
		 //2
		 88.0f,-100.0f,//S
		-88.0f,100.0f,//T2
		 88.0f,100.0f,//A1
		// Lado Direito
		-98.0f,100.0f,//S2
		-88.0f,-100.0f,//A2
		-98.0f,-100.0f,//B2
		//2
		-98.0f,100.0f,//S2
		-88.0f,100.0f,//T2
		-88.0f,-100.0f,//A2
		// Lado Esquerdo
		 88.0f,100.0f,//A1
		 98.0f,100.0f,//B1
		 98.0f,-100.0f,//U
		//2
		 88.0f,100.0f,//A1
		 88.0f,-100.0f,//S
		 98.0f,-100.0f,//U
		//Faixa 01
		-2.0f,98.0f,//M3
		-2.0f,68.0f,//O3
		 2.0f,68.0f,//P3
		 //2
		-2.0f,98.0f,//M3
		 2.0f,98.0f,//N3
		 2.0f,68.0f,//P3
		 //Faixa 02
		-2.0f,58.0f,//Q3
		-2.0f,28.0f,//S3
		 2.0f,28.0f,//T3
		 //2
		-2.0f,58.0f,//Q3
		 2.0f,58.0f,//R3
		 2.0f,28.0f,//T3
		 //Faixa 03
		-2.0f,18.0f,//U3
		-2.0f,-12.0f,//W3
		 2.0f,-12.0f,//Z3
		 //2
		-2.0f,18.0f,//U3
		 2.0f,18.0f,//V3
		 2.0f,-12.0f,//Z3

		 //Faixa 04
		-2.0f,-22.0f,//A4
		-2.0f,-52.0f,//C4
		 2.0f,-52.0f,//D4
		 //2
		-2.0f,-22.0f,//A4
		 2.0f,-22.0f,//B4
		 2.0f,-52.0f,//D4

		 //Faixa 04
		-2.0f,-62.0f,//E4
		-2.0f,-92.0f,//G4
		 2.0f,-92.0f,//H4
		 //2
		-2.0f,-62.0f,//E4
		 2.0f,-62.0f,//F4
		 2.0f,-92.0f,//H4

	};

	GLfloat g_color_buffer_data_pista2[144] = {
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

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

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
	};

		glGenBuffers(1, &vertexbuffer);
	
		glGenBuffers(1, &colorbuffer);

		GLuint MatrixID = glGetUniformLocation(programID, "MatrizCombinada");
		glm::mat3 MatrizCombinada = glm::mat3(1.0f);
		//glm::vec3 position(0.f);
		//glm::mat4 ModelMatrix(1.f);
		//ModelMatrix = glm::translate(ModelMatrix,position);


	do{
		// Limpa a Tela
		glClear(GL_COLOR_BUFFER_BIT);

		// Para definir os Shaders
		glUseProgram(programID);
		//KeyBoard(window, position);
		configLayout(vertexbuffer, colorbuffer);
		KeyboardMovimentObject();
		MatrizCombinada = translation;
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);

		PrintNaTela(g_vertex_buffer_data_pista2,sizeof(g_vertex_buffer_data_pista2), g_color_buffer_data_pista2,sizeof(g_color_buffer_data_pista2));
		PrintNaTela(g_vertex_buffer_data_car,sizeof(g_vertex_buffer_data_car), g_color_buffer_data_car,sizeof(g_color_buffer_data_car));

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
