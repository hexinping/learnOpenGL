#pragma once
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>  
#include <fstream>  
#include <streambuf>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;
class OpenglUtils
{
public:
	static OpenglUtils * getInstance();
	char * readTheFile(string file);
	void bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO, float vertices[], int verticesLenght, bool useEBO, unsigned int *EBO, unsigned int indices[], int indicesLenght);
	void  createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource);
	void  createShaderWithFile(int shaderType, unsigned int *pShader, string file);
	int linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader, unsigned int geoMetryShader = 0);

	GLint getUniformLocation(unsigned int program, const char *name);
	GLint getAttribLocation(unsigned int program, const char *name);

	void useProgram(unsigned int program);

	// 图片文件，纹理格式，多级渐远纹理的级别，纹理单元, 纹理环绕方式，纹理过滤方式
	unsigned int genTexImage2D(const char *file, int type = GL_RGB, int level = 0, int textureUnit = GL_TEXTURE0, int around = GL_REPEAT, int filter = GL_LINEAR);

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	unsigned int loadCubemap(vector<std::string> &faces, bool isRevert = false);
	unsigned int loadTexture(char const * path, int textureUnit = GL_TEXTURE0);

	// uniform工具函数
	void setBool(unsigned int program, const char *name, bool value) const;
	void setInt(unsigned int program, const char *name, int value) const;
	void setFloat(unsigned int program, const char *name, float value) const;

	void setMat2(unsigned int program, const char *name, glm::mat2 &mat);
	void setMat3(unsigned int program, const char *name, glm::mat3 &mat);
	void setMat4(unsigned int program, const char *name, glm::mat4 *mat);

	void setVec2(unsigned int program, const char *name, float x, float y);
	void setVec3(unsigned int program, const char *name, float x, float y, float z);
	void setVec4(unsigned int program, const char *name, float x, float y, float z, float w);
	
	void setVec2(unsigned int program, const char *name, glm::vec2 &value);
	void setVec3(unsigned int program, const char *name, glm::vec3 &value);
	void setVec4(unsigned int program, const char *name, glm::vec4 &value);
	
};

