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

	window = glfwCreateWindow(WIDTH, HEIGHT, "Car Race", NULL, NULL);
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

	// if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //Up
	// {
	// 	translation[0][2] -= 0.00f;
	// 	translation[1][2] += 0.05f;

	// }else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //Down
	// {
	// 	translation[0][2] -= 0.00f;
	// 	translation[1][2] -= 0.05f;

	// }else
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { // Right
        if (translation[0][2] == 0.0) {
            translation[0][2] = 0.5f;
        } else if (translation[0][2] == -0.5) {
            translation[0][2] = 0.0f;
        }
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { // Left
        if (translation[0][2] == 0.0) {
            translation[0][2] = -0.5f;
        } else if (translation[0][2] == 0.5) {
            translation[0][2] = 0.0f;
        }
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
	GLfloat g_vertex_buffer_data_car[] = {
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

	GLfloat g_color_buffer_data_car[] = {
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

	GLfloat g_vertex_buffer_data_car2[] = {
		// Body
		0.0f,90.0f,
		0.0f,75.0f,
		5.0f,75.0f,//D
		 //2
		0.0f,90.0f,
		5.0f,90.0f,//B
		5.0f,75.0f,//D

		//Perna
		-10.0f,85.0f,//E
		-10.0f,80.0f,//F
		 15.0f,80.0f,//J
		 //2
		-10.0f,85.0f,//E
		 15.0f,85.0f,//I
		 15.0f,80.0f,//J

		 //Bracos
		-10.0f,80.0f,//F
		-10.0f,75.0f,//K
		 -5.0f,75.0f,//L
		 //2
		-10.0f,80.0f,//F
		-5.0f,80.0f,//M
		 -5.0f,75.0f,//L
		 //Bracos 2
		 10.0f,80.0f,//P
		 10.0f,75.0f,//N
		 15.0f,75.0f,//O
		 //2
		 10.0f,80.0f,//P
		 15.0f,80.0f,//J
		 15.0f,75.0f,//O
	};

	GLfloat g_color_buffer_data_car2[72] = {
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

	GLfloat g_vertex_buffer_data_pista2[] = {
        // Muro direita
        92.5f, 100.0f,
        82.5f, 100.0f,
        82.5f, 80.0f,

        92.5f, 100.0f,
        92.5f, 80.0f,
        82.5f, 80.0f,

        92.5f, 70.0f,
        82.5f, 70.0f,
        82.5f, 30.0f,

        92.5f, 70.0f,
        92.5f, 30.0f,
        82.5f, 30.0f,

        92.5f, 20.0f,
        82.5f, 20.0f,
        82.5f, 0.0f,

        92.5f, 20.0f,
        92.5f, 0.0f,
        82.5f, 0.0f,

        92.5f, 0.0f,
        82.5f, 0.0f,
        92.5f, -20.0f,

        82.5f, 0.0f,
        92.5f, -20.0f,
        82.5f, -20.0f,

        82.5f, -30.0f,
        92.5f, -70.0f,
        82.5f, -70.0f,

        92.5f, -30.0f,
        82.5f, -30.0f,
        92.5f, -70.0f,

        82.5f, -80.0f,
        92.5f, -100.0f,
        82.5f, -100.0f,

        92.5f, -80.0f,
        82.5f, -80.0f,
        92.5f, -100.0f,

        // Muro Esquerda
        -92.5f, 100.0f,
        -82.5f, 100.0f,
        -82.5f, 80.0f,

        -92.5f, 100.0f,
        -92.5f, 80.0f,
        -82.5f, 80.0f,

        -92.5f, 70.0f,
        -82.5f, 70.0f,
        -82.5f, 30.0f,

        -92.5f, 70.0f,
        -92.5f, 30.0f,
        -82.5f, 30.0f,

        -92.5f, 20.0f,
        -82.5f, 20.0f,
        -82.5f, 0.0f,

        -92.5f, 20.0f,
        -92.5f, 0.0f,
        -82.5f, 0.0f,

        -92.5f, 0.0f,
        -82.5f, 0.0f,
        -92.5f, -20.0f,

        -82.5f, 0.0f,
        -92.5f, -20.0f,
        -82.5f, -20.0f,

        -82.5f, -30.0f,
        -92.5f, -70.0f,
        -82.5f, -70.0f,

        -92.5f, -30.0f,
        -82.5f, -30.0f,
        -92.5f, -70.0f,

        -82.5f, -80.0f,
        -92.5f, -100.0f,
        -82.5f, -100.0f,

        -92.5f, -80.0f,
        -82.5f, -80.0f,
        -92.5f, -100.0f,

        //Pista 1 (Esquerda)
        -75.0f, 100.0f,
        -75.0f, -100.0f,
        -25.0f, 100.0f,

        -75.0f, -100.0f,
        -25.0f, -100.0f,
        -25.0f, 100.0f,

        //Pista 2 (Meio)
        -25.0f, 100.0f,
        -25.0f, -100.0f,
        25.0f, 100.0f,

        -25.0f, -100.0f,
        25.0f, -100.0f,
        25.0f, 100.0f,

        //Pista 3 (Direita)
        25.0f, 100.0f,
        25.0f, -100.0f,
        75.0f, 100.0f,

        25.0f, -100.0f,
        75.0f, -100.0f,
        75.0f, 100.0f,

        // Faixa 01 - Esquerda (centro vertical)
        -27.0f, 15.0f,
        -27.0f, -15.0f,
        -23.0f, -15.0F,

        -27.0f, 15.0f,
        -23.0f, 15.0f,
        -23.0f, -15.0F,

        // Faixa 02 - Esquerda
        -27.0f, 55.0f,
        -27.0f, 25.0f,
        -23.0f, 25.0F,

        -27.0f, 55.0f,
        -23.0f, 55.0f,
        -23.0f, 25.0F,

        // Faixa 03 - Esquerda
        -27.0f, 95.0f,
        -27.0f, 65.0f,
        -23.0f, 65.0F,

        -27.0f, 95.0f,
        -23.0f, 95.0f,
        -23.0f, 65.0F,

        // Faixa 04 - Esquerda
        -27.0f, -55.0f,
        -27.0f, -25.0f,
        -23.0f, -25.0F,

        -27.0f, -55.0f,
        -23.0f, -55.0f,
        -23.0f, -25.0F,

        // Faixa 05 - Esquerda
        -27.0f, -95.0f,
        -27.0f, -65.0f,
        -23.0f, -65.0F,

        -27.0f, -95.0f,
        -23.0f, -95.0f,
        -23.0f, -65.0f,

        // Faixa 01 - Direita (centro vertical)
        27.0f, 15.0f,
        27.0f, -15.0f,
        23.0f, -15.0F,

        27.0f, 15.0f,
        23.0f, 15.0f,
        23.0f, -15.0F,

        // Faixa 02 - Direita
        27.0f, 55.0f,
        27.0f, 25.0f,
        23.0f, 25.0F,

        27.0f, 55.0f,
        23.0f, 55.0f,
        23.0f, 25.0F,

        // Faixa 03 - Direita
        27.0f, 95.0f,
        27.0f, 65.0f,
        23.0f, 65.0F,

        27.0f, 95.0f,
        23.0f, 95.0f,
        23.0f, 65.0F,

        // Faixa 04 - Direita
        27.0f, -55.0f,
        27.0f, -25.0f,
        23.0f, -25.0F,

        27.0f, -55.0f,
        23.0f, -55.0f,
        23.0f, -25.0F,

        // Faixa 05 - Direita
        27.0f, -95.0f,
        27.0f, -65.0f,
        23.0f, -65.0F,

        27.0f, -95.0f,
        23.0f, -95.0f,
        23.0f, -65.0f
    };

	GLfloat g_color_buffer_data_pista2[] = {
        // Muro Direita
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

        // Muro Esquerda
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,
		0.4f,  0.4,  0.4f,

        // Pista 1 (Esquerda)
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

        // Pista 2 (Meio)
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

        // Pista 3 (Direita)
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,
		0.0f,  0.0f,  0.0f,

        // Faixas Pontilhadas (meio pistas)
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
		MatrizCombinada = glm::mat3(1.0f);
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);
		PrintNaTela(g_vertex_buffer_data_pista2,sizeof(g_vertex_buffer_data_pista2), g_color_buffer_data_pista2,sizeof(g_color_buffer_data_pista2));

		MatrizCombinada = translation;
		glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);
		PrintNaTela(g_vertex_buffer_data_car,sizeof(g_vertex_buffer_data_car), g_color_buffer_data_car,sizeof(g_color_buffer_data_car));
		PrintNaTela(g_vertex_buffer_data_car2,sizeof(g_vertex_buffer_data_car2), g_color_buffer_data_car2,sizeof(g_color_buffer_data_car2));

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
