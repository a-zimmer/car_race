#include <bits/stdc++.h>

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

const GLint WIDTH = 1280, HEIGHT = 768;
const GLfloat R = 0.0f, G = 0.0f, B = 0.3f, A = 0.0f;


int initWindow(){
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3.3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow( WIDTH, HEIGHT, "Color triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	// Aplica o contexto atual da máquina de estados para Janela Atual
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Necessário para core profile
	//deve ativar antes da chamada glewInit()
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glClearColor(R, G, B, A);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//  Mostra o fragmento que estiver mais próximo da câmera Z-Buffer
	glDepthFunc(GL_LESS);
}

void destroyWindows(GLuint vertexbuffer, GLuint colorbuffer, GLuint VertexArrayID, GLuint programID){
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}


void configLayout(GLuint vertexbuffer, GLuint colorbuffer){

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
}


int main( void )
{
	initWindow();

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );


	static const GLfloat g_vertex_buffer_data[] = { 
		-0.1f, 0.0f,
		0.1f, 0.0f,
		0.0f, 0.1f,
	};

	static const GLfloat g_vertex_buffer_data2[] = { 
		-0.4f, 0.0f,
		-0.2f, 0.0f,
		-0.3f, 0.1f,
	};

	static const GLfloat g_vertex_buffer_data3[] = { 
		0.4f, 0.0f,
		0.2f, 0.0f,
		0.3f, 0.1f,
	};


	static const GLfloat g_color_buffer_data[] = { 
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
	};

	static const GLfloat g_color_buffer_data2[] = { 
		0.0f,  0.0f,  1.0f,
		0.0f,  1.0f,  0.0f,
		1.0f,  0.0f,  0.0f,
	};

	static const GLfloat g_color_buffer_data3[] = { 
		0.0f,  0.0f,  1.0f,
		0.0f,  0.0f,  1.0f,
		1.0f,  1.0f,  0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	
	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		
		configLayout(vertexbuffer, colorbuffer);
		

		//Triangulo 1
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		//color RGB
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
		//vertexs
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data)/sizeof(GLfloat)); 
		
		//Triangulo 2
		//color RGB
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data2), g_color_buffer_data2, GL_STATIC_DRAW);
		//vertexs
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data2)/sizeof(GLfloat)); 

		//Triangulo 3
		//color RGB
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data3), g_color_buffer_data3, GL_STATIC_DRAW);
		//vertexs
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data3), g_vertex_buffer_data3, GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(g_vertex_buffer_data3)/sizeof(GLfloat)); 


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS
		&& glfwWindowShouldClose(window) == 0);

	destroyWindows(vertexbuffer, colorbuffer, VertexArrayID, programID);

	return 0;
}

