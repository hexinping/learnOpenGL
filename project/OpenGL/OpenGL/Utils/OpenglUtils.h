#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>  
#include <fstream>  
#include <streambuf>
using namespace std;
class OpenglUtils
{
public:
	static OpenglUtils * getInstance();
	char * readTheFile(string file);
	void bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO, float vertices[], int verticesLenght, bool useEBO, unsigned int *EBO, unsigned int indices[], int indicesLenght);
	void  createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource);
	void  createShaderWithFile(int shaderType, unsigned int *pShader, string file);
	int linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader);

	GLint getUniformLocation(unsigned int program, const char *name);
	GLint getAttribLocation(unsigned int program, const char *name);

};

