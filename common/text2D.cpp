#include <bits/stdc++.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "texture.hpp"

#include "text2D.hpp"

unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;

void initText2D(const char * texturePath){

	// Initialize texture
	Text2DTextureID = loadDDS(texturePath);

	// Initialize VBO
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	Text2DShaderID = LoadShaders( "TextVertexShader.vertexshader", "TextVertexShader.fragmentshader" );

	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation( Text2DShaderID, "myTextureSampler" );

}

void printText2D(const char * text, int x, int y, int size){

	unsigned int length = strlen(text);
	//printf("%u\n", length);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for ( unsigned int i=0 ; i<length ; i++ ){
		
		glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
		glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
		glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );


		/*printf("vertex_up_left X: %d Y: %d\n", x+i*size, y+size);
		printf("vertex_up_right X: %d Y: %d\n", x+i*size+size, y+size);
		printf("vertex_down_right X: %d Y: %d\n", x+i*size+size, y);
		printf("vertex_down_left X: %d Y: %d\n", x+i*size, y);
		printf("\n");*/

		vertices.push_back(vertex_up_left   );
		vertices.push_back(vertex_down_left );
		vertices.push_back(vertex_up_right  );

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;
		
		/*printf("%d %d %f\n", character,  character%16, (character%16)/16.0f);
		printf("%d %d %f\n", character,  character/16, (character/16)/16.0f);*/

		glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
		glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
		glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
		glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );

		/*printf("uv_up_left X: %f Y: %f\n", uv_x, uv_y);
		printf(" uv_up_right X: %f Y: %f\n", uv_x+1.0f/16.0f, uv_y);
		printf("uv_down_right X: %f Y: %f\n", uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f));
		printf("uv_down_left X: %f Y: %f\n", uv_x , (uv_y + 1.0f/16.0f));
		printf("\n");*/

		UVs.push_back(uv_up_left   );
		UVs.push_back(uv_down_left );
		UVs.push_back(uv_up_right  );

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	//printf("\n\n\n");
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(Text2DShaderID);

	// Bind texture in GL_TEXTURE+i and default is GL_TEXTURE0 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Text2DUniformID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glVertexAttribPointer(
		0, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		0, 
		(void*)0 
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glVertexAttribPointer(
		1, 
		2, 
		GL_FLOAT, 
		GL_FALSE, 
		0, 
		(void*)0 
	);

	//Habilita a mistura de cores
	glEnable(GL_BLEND);
	//Especifica como o red, green, blue, and alpha de origem(textura) e destino(openGl) são misturados
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

	//desabilita a mistura de cores
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void cleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	// Delete shader
	glDeleteProgram(Text2DShaderID);
}
