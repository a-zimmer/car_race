#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
//#include "common/song.hpp"

const GLint WIDTH = 800, HEIGHT = 600;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;
GLuint colorbuffer, vertexbuffer;
double xposMouse, yposMouse;
int widthWindow, heightWindow, randomPosition = 1;
int direita = 1;
int esquerda = 0;

glm::mat3 translation = glm::mat3(1.0f);
glm::vec3 vetorCores = glm::vec3(0.0f);
glm::mat3 escala = glm::mat3(1.0f);
glm::mat3 rotacao = glm::mat3(1.0f);
glm::mat3 pistaMovement = glm::mat3(1.0f);
glm::mat3 objectTranslation = glm::mat3(1.0f);



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



void KeyboardMovementObject(double deltaTime, double deltaTime2){
	glfwGetCursorPos(window, &xposMouse, &yposMouse);
	glfwGetWindowSize(window,&widthWindow, &heightWindow);
	double horizontal = double(xposMouse * 2 - widthWindow)/double(widthWindow);
	double vertical = double(heightWindow - yposMouse * 2)/double(heightWindow);
	//printf("%lf %lf\n",xposMouse,yposMouse);
	//printf("%lf %lf\n",horizontal,vertical);
	translation[1][2] = -0.7; //Inicia o carro la em baixo na posiçao -0.7;
	//objectTranslation[1][2] = -0.1f;

	//Colocar FLAG DIREITA E ESQUERDA;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && direita == 1) { // Right
        if (translation[0][2] == 0.0) {
            translation[0][2] = 0.5f;
        } else if (translation[0][2] == -0.5) {
            translation[0][2] = 0.0f;
        }
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && esquerda == 0) { // Left
        if (translation[0][2] == 0.0) {
            translation[0][2] = -0.5f;
        } else if (translation[0][2] == 0.5) {
            translation[0][2] = 0.0f;
        }
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // Left
        if (pistaMovement[1][2] == 0.0) { // Testes de Movementação da pista
        	pistaMovement[1][2] -= 0.5f;
        	printf("MACONHA\n");
    } else if (pistaMovement[1][2] == -0.5f){
    		pistaMovement[1][2] = 0.0f;
    		printf("MACONHA2\n");
    	}
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { // DOWN
        	objectTranslation[0][2] -= 0.00f;//TESTE pr baixo
			objectTranslation[1][2] -= 0.05f;
    }
}
void trackAnimation(double deltaTime, double deltaTime2) {
	if (pistaMovement[1][2] == 0.0) {
        	pistaMovement[1][2] -= 0.5f;
        	//printf("Pista Move\n");
    } else if (pistaMovement[1][2] == -0.5f){
    		pistaMovement[1][2] = 0.0f;
    		//printf("Pista Move 2\n");
    	}

}

void turboAnimation(double deltaTime, double deltaTime2) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (translation[1][2] == -0.7f) {
            translation[1][2] += 0.5f;
            printf("Car Turbo UP\n");
        }
	}
	if(translation[1][2] == 0.5f) { //Não necessario pois o mesmo esta na função KeyboardMovementObject(); Porem está muito rapido os frames
		translation[1][2] = -0.7f;
		printf("Car Turbo DOWN\n");
	}
}
float velocidadeObject = 0.005f;
int nCarrinhos = 1;
void objectAnimation() {
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        printf("randomPosition: %d\n", randomPosition);

		if (objectTranslation[1][2] < -2.0f) {
			nCarrinhos += 1;
            randomPosition = rand() % 3;
			objectTranslation[1][2] = -0.1f;
			velocidadeObject += sqrt(nCarrinhos)/5000;
			printf("TEST IF RANDOM\n");
		}
        if (randomPosition == 2) {
        	objectTranslation[0][2]  = 0.5f;
			objectTranslation[1][2] -= velocidadeObject;
            printf("Object Movement 2.\n");
        }

        if (randomPosition == 1) {
        	objectTranslation[0][2]  = 0.00f;
			objectTranslation[1][2] -= velocidadeObject;
			
            printf("Object Movement 1.\n");
        }

        if (randomPosition == 0) {
        	objectTranslation[0][2]  = -0.5f;
			objectTranslation[1][2] -= velocidadeObject;
			printf("Velocidade do objeto: %f \n", velocidadeObject);
            printf("Object Movement 0.\n");
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

glm::vec4 getCarrinhoBox(std::vector<glm::vec2> objeto) {
	glm::vec4 box;
	float xMax = objeto[0].x, yMax = objeto[0].y, xMin = objeto[0].x, yMin = objeto[0].y;

	for(int point = 0; point < objeto.size(); point++) {
		//printf("Objeto: %f\n", objeto[point].x);
			if (xMax > objeto[point].x) {
				xMax = objeto[point].x;
			}if (xMin < objeto[point].x){
				xMin = objeto[point].x;
			}if (yMax > objeto[point].y){
				yMax = objeto[point].y;
			}if (yMin < objeto[point].y){
				yMin = objeto[point].y;
			};
		}
		printf("IMPRIME: \nxMax:%f\nyMax:%f\nxMin:%f\nyMin:%f\n",xMax,yMax,xMin,yMin);
		box = glm::vec4(xMax,yMax,xMin,yMin);
	return box;
}

int intersect(glm::vec4 carrinhoUm,glm::vec4 carrinhoDois, glm::mat3 matrizDeTranslation, glm::mat3 matrizDeTranslation2) {
//	xMax = [0];
//	yMax = [1];
//	xMin = [2];
//	yMin = [3];
if ((carrinhoUm[2] + matrizDeTranslation[0][2] <= carrinhoDois[0] + matrizDeTranslation2[0][2])
 	&& (carrinhoUm[0] + matrizDeTranslation[0][2] >= carrinhoDois[2] + matrizDeTranslation2[0][2]) &&
    (carrinhoUm[3] + matrizDeTranslation[0][2] <= carrinhoDois[1] + matrizDeTranslation2[0][2])
    && (carrinhoUm[1] + matrizDeTranslation[0][2] >= carrinhoDois[3] + matrizDeTranslation2[0][2])){
    	return 1;
    };
};

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


	double lastTime2;
	double lastTime = lastTime2 = glfwGetTime();
 	int nbFrames = 0, nbFrames2=0;
	// Measure speed
	double deltaTime = 0,deltaTime2 = 0,currentTime;
	translation[1][2] = -0.7; //Inicia o carro la em baixo na posiçao -0.7;
	objectTranslation[1][2] = -0.1;
	do{
		bool colision = false;
		int score = 0;
		//Medindo Velocidade
		currentTime = glfwGetTime();
		nbFrames++;
		deltaTime = currentTime - lastTime;
		deltaTime2 = currentTime - lastTime2;
		 if (deltaTime  >= 0.09 ){ // If last prinf() was more than 5 sec ago
		// 	 // printf and reset timer
		 	// printf("%d \n",nbFrames );
		 	// printf("%f ms/frame/deltaTime\n", 1000.0/double(nbFrames));

		 	 nbFrames = 0;
		 	 trackAnimation(deltaTime, deltaTime2);
		 	 lastTime += 0.09;
		 	 
		 }

		 if (deltaTime2  >= 0.8){ // If last prinf() was more than 5 sec ago
		// 	 // printf and reset timer
		 	// printf("%d \n",nbFrames2 );
		 	 //printf("%f ms/frame/deltaTime2\n", 1000.0/double(nbFrames2));

		 	 nbFrames2 = 0;
		 	 lastTime2 += 0.8;
		 	 esquerda = 0;
		 	 direita = 1;
		 	 KeyboardMovementObject(deltaTime, deltaTime2);
		 }	

		 esquerda = 1;
		 direita = 0;
		// Limpa a Tela
		glClear(GL_COLOR_BUFFER_BIT);

		// Para definir os Shaders
		glUseProgram(programID);
		//KeyBoard(window, position);
		configLayout(vertexbuffer, colorbuffer);

		//KeyboardMovementObject(deltaTime, deltaTime2);
		//trackAnimation(deltaTime, deltaTime2);
		turboAnimation(deltaTime, deltaTime2);
		objectAnimation();


		MatrizCombinada = glm::mat3(1.0f);
		drawModel(verticesPistas, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);
		//MatrizCombinada = pistaMovement;
		drawModel(verticesMuro, MatrizCombinada, MatrixID, 0.4, 0.4, 0.4);
		MatrizCombinada = pistaMovement;
		drawModel(verticesFaixas, MatrizCombinada, MatrixID, 1.0, 1.0, 1.0);


		

		MatrizCombinada = translation;
		drawModel(verticesCar1, MatrizCombinada, MatrixID, 1.0, 0.0, 0.0);
		glm::vec4 carrinhoOne = getCarrinhoBox(verticesCar1);
		MatrizCombinada = objectTranslation;
		drawModel(verticesCar2, MatrizCombinada, MatrixID, 0.0, 0.0, 1.0);
 		glm::vec4 carrinhoTwo = getCarrinhoBox(verticesCar2);
		//Som Colisão
		if(intersect(carrinhoOne, carrinhoTwo, translation, objectTranslation)) {
			printf("GAME OVER!!!\n");
		}else {
			score++;
		};

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
