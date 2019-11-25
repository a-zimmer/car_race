#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
#include "common/text2D.hpp"
//FMOD bib audio manipulator
#include "common/song.hpp"
// Include AntTweakBar
#include "AntTweakBar/include/AntTweakBar.h"


#define NUM_SHAPES 3
typedef enum { SHAPE_TEAPOT=1, SHAPE_TORUS, SHAPE_CONE } Shape;
Shape g_CurrentShape = SHAPE_TORUS;

const GLint WIDTH = 800, HEIGHT = 600;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;
float CR, CG, CB;

bool gLookAtOther = true;
double zoom = 1.0f, g_Rotation = 0, g_LightDirection[] = { -0.57735f, -0.57735f, -0.57735f };

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
static std::vector<char *> gPathList;

//Menu
bool ativo = false;
int g_CurrentPlaca = 2;
vec3 g_MatAmbient= vec3(1.0f, 0.0f, 0.0f);

void adicionaBarras() {
	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(WIDTH, HEIGHT);//Alterar tamanho da Janela

	TwBar * bar = TwNewBar("MENU");
	TwSetParam(bar, NULL, "position", TW_PARAM_CSTRING, 1, "10 10");
	TwSetParam(bar, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
    TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLFW and OpenGL.' "); // Message added to the help bar.

    TwAddVarRW(bar, "Ativar Animacao", TW_TYPE_BOOL8 , &ativo, NULL);
    TwAddVarRO(bar, "Game", TW_TYPE_BOOLCPP, &ativo, " true='Andando' false='Pausado' ");
    TwAddVarRW(bar, "Cor Carrinho", TW_TYPE_COLOR3F, &g_MatAmbient,"colormode=rgb");
   {
        // vetorOpcao associa a placa com o valor da label
        TwEnumVal vetorOpcao[2] = { {1, "Carrinho Azul"}, {2, "Carrinho Preto"}};
        // Create a type for the enum vetorOpcao
        TwType tipoPlaca = TwDefineEnum("TipoCarrinho", vetorOpcao, 2);
        // add 'g_CurrentShape' to 'bar': this is a variable of type ShapeType. Its key shortcuts are [<] and [>].
        TwAddVarRW(bar, "Carrinho", tipoPlaca, &g_CurrentPlaca, " keyIncr='<' keyDecr='>' help='Selecione a placa.' ");
    }
}


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
	adicionaBarras();

	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(window, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(window, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
	glfwSetCharCallback(window, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//Alterado

    // Set the mouse at the center of the screen
    glfwPollEvents();

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
	// Delete the text's VBO, the shader and the texture
	cleanupText2D();
	//kill song

	killSong();
	TwTerminate();
	glfwTerminate();
}

void KeyboardMovementObject(double deltaTime, double deltaTime2){
	glfwGetCursorPos(window, &xposMouse, &yposMouse);
	glfwGetWindowSize(window,&widthWindow, &heightWindow);
	double horizontal = double(xposMouse * 2 - widthWindow)/double(widthWindow);
	double vertical = double(heightWindow - yposMouse * 2)/double(heightWindow);
	translation[1][2] = -0.7; //Inicia o carro la em baixo na posiçao -0.7;

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
    }
}

void trackAnimation() {
	if (pistaMovement[1][2] == 0.0) {
        	pistaMovement[1][2] -= 0.5f;
        	//printf("Pista Move\n");
    } else if (pistaMovement[1][2] == -0.5f){
    		pistaMovement[1][2] = 0.0f;
    		//printf("Pista Move 2\n");
    	}
}

void trackAnimation2() {
	if(pistaMovement[1][2] < 0.8 && pistaMovement[1][2] > -0.1) {
			pistaMovement[1][2] -=0.02;
	}else{
		pistaMovement[1][2] = 0.1f;
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
    if (objectTranslation[1][2] < -2.0f) {
        randomPosition = rand() % 3;
        objectTranslation[1][2] = -0.1f;
        nCarrinhos++;
    }

    velocidadeObject = sqrt(nCarrinhos)/100;
    // printf("velocidadeObject: %f\n", velocidadeObject);
    if (randomPosition == 2) {
        objectTranslation[0][2]  = 0.5f;
        objectTranslation[1][2] -= velocidadeObject;
    }

    if (randomPosition == 1) {
        objectTranslation[0][2]  = 0.00f;
        objectTranslation[1][2] -= velocidadeObject;
    }

    if (randomPosition == 0) {
        objectTranslation[0][2]  = -0.5f;
        objectTranslation[1][2] -= velocidadeObject;
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

char *MediaPath(const char *fileName)
{
    char *filePath = (char *)calloc(256, sizeof(char));

    ssize_t len = readlink("/proc/self/exe", filePath, 256);
    assert(len != -1);

    char *filePathEnd = strrchr(filePath, '/');
    assert (filePathEnd != NULL);

    filePathEnd++; // Move past the last slash
    filePathEnd[0] = '\0';

    strcat(filePath, "common/media/");
    strcat(filePath, fileName);
    gPathList.push_back(filePath);

    return filePath;
}

void controlSong(){
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		decreaseVolume();
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		increaseVolume();
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		//FMOD_PlayPause(0);
		killSong();
		FMOD_Config(MediaPath("CarroAndando.mp3"));
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		FMOD_PlayPause(1);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		killSong();
		FMOD_Config(MediaPath("stereo.ogg"));
		killSong();
		FMOD_Config(MediaPath("standrews.wav"));

	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		killSong();
		FMOD_Config(MediaPath("swish.wav"));

	}
}

glm::vec4 getCarrinhoBox(std::vector<glm::vec2> objeto) {
	glm::vec4 box;
	float xMax = objeto[0].x, yMax = objeto[0].y, xMin = objeto[0].x, yMin = objeto[0].y;

	for(int point = 0; point < objeto.size(); point++) {
        if (xMax < objeto[point].x) {
            xMax = objeto[point].x;
        }

        if (xMin > objeto[point].x) {
            xMin = objeto[point].x;
        }

        if (yMax < objeto[point].y) {
            yMax = objeto[point].y;
        }

        if (yMin > objeto[point].y) {
            yMin = objeto[point].y;
        }
    }

    // printf("IMPRIME: \nxMax:%f\nyMax:%f\nxMin:%f\nyMin:%f\n",xMax,yMax,xMin,yMin);
    box = glm::vec4(xMax,yMax,xMin,yMin);
	return box;
}

int intersect(glm::vec4 carrinhoUm,glm::vec4 carrinhoDois, glm::mat3 matrizDeTranslation, glm::mat3 matrizDeTranslation2) {
//	xMax = [0];
//	yMax = [1];
//	xMin = [2];
//	yMin = [3];
    return ((carrinhoUm[2] + matrizDeTranslation[0][2] >= carrinhoDois[0] + matrizDeTranslation2[0][2])
        && (carrinhoUm[0] + matrizDeTranslation[0][2] <= carrinhoDois[2] + matrizDeTranslation2[0][2]) &&
        (carrinhoUm[3] + matrizDeTranslation[1][2] >= carrinhoDois[1] + matrizDeTranslation2[1][2])
        && (carrinhoUm[1] + matrizDeTranslation[1][2] <= carrinhoDois[3] + matrizDeTranslation2[1][2]));
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
	initText2D( "Holstein.DDS" );

	std::vector<glm::vec2> verticesCar1 = loadModel("data/car1.txt");
	std::vector<glm::vec2> verticesCar2 = loadModel("data/car2.txt");
	std::vector<glm::vec2> verticesMuro = loadModel("data/muro.txt");
	std::vector<glm::vec2> verticesPistas = loadModel("data/pistas.txt");
	std::vector<glm::vec2> verticesFaixas = loadModel("data/faixas.txt");
	std::vector<glm::vec2> verticesTelaFinal = loadModel("data/telaFinal.txt");
	std::vector<glm::vec2> verticesTelaInicial = loadModel("data/telaInicial.txt");

	FMOD_Config(MediaPath("standrews.wav"));
	FMOD_PlayPause(1);

//AAA
	double lastTime2,lastTime3;
	double lastTime = lastTime2 = lastTime3 = glfwGetTime();
 	int nbFrames = 0, nbFrames2=0, nbFrames3=0;
	// Measure speed
	double deltaTime = 0,deltaTime2 = 0,deltaTime3 = 0,currentTime;
	translation[1][2] = -0.7; //Inicia o carro la em baixo na posiçao -0.7;
	objectTranslation[1][2] = -0.1;

	bool joguinho = false;
	do{
		controlSong();
		bool colision = false;
		int score = 0;
		//Medindo Velocidade
		currentTime = glfwGetTime();
		nbFrames++;
		deltaTime = currentTime - lastTime;
		deltaTime2 = currentTime - lastTime2;
		deltaTime3 = currentTime - lastTime3;

		 if (deltaTime2  >= 0.25) {
		 	 nbFrames2 = 0;
		 	 lastTime2 += 0.25;
		 	 esquerda = 0;
		 	 direita = 1;
		 	 KeyboardMovementObject(deltaTime, deltaTime2);
		 }
		 if (deltaTime3  >= 0.033) {
		 	 nbFrames3 = 0;
		 	 lastTime3 += 0.033;
		 	 if(ativo) {
		 	 	objectAnimation();
		 	 }
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
        if(joguinho == false) {
			drawModel(verticesTelaInicial, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);
			char telaInicial[256];
			sprintf(telaInicial,"CAR RACE");
			printText2D(telaInicial, 130, 350, 70);
			sprintf(telaInicial,"PRESSIONE ENTER PARA JOGAR");
			printText2D(telaInicial, 10, 280, 30);
		}
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			joguinho = true;
			ativo = true;
		}

		if (deltaTime  >= 0.04 ){ // If last prinf() was more than 5 sec ago
		 	 nbFrames = 0;
			if(ativo) {
				//trackAnimation();
				trackAnimation2();
			}
			 lastTime += 0.04;
		}

		if(joguinho) {
			MatrizCombinada = glm::mat3(1.0f);
			drawModel(verticesPistas, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);
			MatrizCombinada = pistaMovement;
			drawModel(verticesMuro, MatrizCombinada, MatrixID, 0.4, 0.4, 0.4);
			drawModel(verticesFaixas, MatrizCombinada, MatrixID, 1.0, 1.0, 1.0);

			MatrizCombinada = translation;
			drawModel(verticesCar1, MatrizCombinada, MatrixID, 1.0, 0.0, 0.0);
			glm::vec4 carrinhoOne = getCarrinhoBox(verticesCar1);
			MatrizCombinada = objectTranslation;
			drawModel(verticesCar2, MatrizCombinada, MatrixID, 0.0, 0.0, 1.0);
 			glm::vec4 carrinhoTwo = getCarrinhoBox(verticesCar2);

			//Som Colisão, Score, GAMEOVER
			if(intersect(carrinhoOne, carrinhoTwo, translation, objectTranslation)) {
                killSong();
		        FMOD_Config(MediaPath("gameOver.mp3"));
                drawModel(verticesTelaFinal, MatrizCombinada, MatrixID, 0.0, 0.0, 0.0);
                char telaFinal[256];
                sprintf(telaFinal,"GAME OVER");
                printText2D(telaFinal, 100, 300, 70);

                joguinho = false;
                ativo = false;
			} else {
				score++;
			};
            char text[256];
			TwDraw();
			sprintf(text,"Score: %d",score);
			printText2D(text, 525, 560, 25);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& !glfwWindowShouldClose(window));
	destroyWindows(vertexbuffer, VertexArrayID, programID);
	return 0;
}
