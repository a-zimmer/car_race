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
GLuint colorbuffer, vertexbuffer;
double xposMouse, yposMouse;
int widthWindow, heightWindow;

glm::mat3 translation = glm::mat3(1.0f);
glm::vec3 vetorCores = glm::vec3(0.0f);
glm::mat3 escala = glm::mat3(1.0f);
glm::mat3 rotacao = glm::mat3(1.0f);

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



void KeyboardMovimentObject(){
	glfwGetCursorPos(window, &xposMouse, &yposMouse);
	glfwGetWindowSize(window,&widthWindow, &heightWindow);
	double horizontal = double(xposMouse * 2 - widthWindow)/double(widthWindow);
	double vertical = double(heightWindow - yposMouse * 2)/double(heightWindow);
	printf("%lf %lf\n",xposMouse,yposMouse);
	printf("%lf %lf\n",horizontal,vertical);


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

std::vector<glm::vec3> colorir(int tam, float R, float G, float B){
	int i;
	//alocação dinâmica
	std::vector<glm::vec3> vertexColor;

	for(i=0;i<tam;i++){
		glm::vec3 color;
		color.r = R;
		color.g = G;
		color.b = B;
		vertexColor.push_back(color); 
	}	
	return vertexColor;
}

std::vector<glm::vec2> loadModel(const char *path){
	FILE * file = fopen(path, "r");
	std::vector<glm::vec2> vertices;
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		fclose(file);
		getchar();
		return vertices;
	}
	
	while( 1 ){
		glm::vec2 vertex;
		int res = fscanf(file, "%f %f\n", &vertex.x, &vertex.y);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		vertices.push_back(vertex);
	}
	fclose(file);
	return vertices;
}

void drawModel(std::vector<glm::vec2> vertices, glm::mat3 MatrizCombinada,
 	GLuint MatrixID, GLfloat R, GLfloat G, GLfloat B){
	
	glUniformMatrix3fv(MatrixID, 1, GL_TRUE, &MatrizCombinada[0][0]);
	//Triangulo
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//color trianguloRGB
	std::vector<glm::vec3> color = colorir(vertices.size(), R, G, B);
	glBufferData(GL_ARRAY_BUFFER, color.size()* sizeof(glm::vec3), &color[0], GL_STATIC_DRAW);
	
	//vertexs
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()* sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
}	

int main(void)
{
	initWindow();

	GLuint VertexArrayID; //conexão com a placa de video
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
	std::vector<glm::vec3> color;



	glGenBuffers(1, &vertexbuffer);

	glGenBuffers(1, &colorbuffer);

	GLuint MatrixID = glGetUniformLocation(programID, "MatrizCombinada");
	glLineWidth(5.0f);
	glm::mat3 MatrizCombinada = glm::mat3(1.0f);

	std::vector<glm::vec2> verticesCar1 = loadModel("data/car1.txt");
	std::vector<glm::vec2> verticesCar2 = loadModel("data/car2.txt");
	std::vector<glm::vec2> verticesMuro = loadModel("data/muro.txt");
	std::vector<glm::vec2> verticesPistas = loadModel("data/pistas.txt");
	std::vector<glm::vec2> verticesFaixas = loadModel("data/faixas.txt");
	
	do{
		// Limpa a Tela
		glClear(GL_COLOR_BUFFER_BIT);

		// Para definir os Shaders
		glUseProgram(programID);
		//KeyBoard(window, position);
		configLayout(vertexbuffer, colorbuffer);
		KeyboardMovimentObject();
		MatrizCombinada = glm::mat3(1.0f);
		drawModel(verticesMuro, MatrizCombinada, MatrixID, 0.4, 0.4, 0.4);
		drawModel(verticesPistas, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);
		drawModel(verticesFaixas, MatrizCombinada, MatrixID, 1.0, 1.0, 1.0);


		MatrizCombinada = translation;
		drawModel(verticesCar1, MatrizCombinada, MatrixID, 1.0, 0.0, 0.0);
		drawModel(verticesCar2, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
