#include "OpenglStateMultTextureTransparent.h"
#include <vector>
#include <map>

bool OpenglStateMultTextureTransparent::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};



	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->genTexImage2D("resource/container.jpg", GL_RGB, 0, GL_TEXTURE0, GL_REPEAT, GL_LINEAR);
	_texture0 = genTexImage2D("resource/window.png", GL_RGBA, 0, GL_TEXTURE0, GL_CLAMP_TO_EDGE, GL_LINEAR);

	__super::initRendCommand();


	/*
		设置uniform变量有两种方法

		第一种：在initRendCommand后使用一次glUseProgram(_shaderProgram)，这个时候赋值给uniform变量
		第二种：在主循环rendeCommand里不断设置
	
	*/
	//设置纹理单元
	_glUtils->useProgram(_shaderProgram);// 先使用这个着色器程序对象才能设置uniform变量


	//通过使用glUniform1i设置采样器，我们保证了每个uniform采样器对应着正确的纹理单元
	setInt(_shaderProgram, "texture1", 0); // 这里的0就对应了前面的GL_TEXTUR
	
	//开启混合								   
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}
bool OpenglStateMultTextureTransparent::isUseEBORender()
{
	return false;
}

bool  OpenglStateMultTextureTransparent::isUsePlane()
{
	return true;
}

void OpenglStateMultTextureTransparent::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();
	
	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "texture1", 0);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);	

	


	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{


		vector<glm::vec3> vegetation
		{
			glm::vec3(-1.5f, 0.0f, -0.48f),
			glm::vec3(1.5f, 0.0f, 0.51f),
			glm::vec3(0.0f, 0.0f, 0.7f),
			glm::vec3(-0.3f, 0.0f, -2.3f),
			glm::vec3(0.5f, 0.0f, -0.6f)
		};

		//存入map中默认按键值升值排序 
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			float distance = glm::length(cameraPos - vegetation[i]);
			sorted[distance] = vegetation[i];
		}

		//从远到近渲染
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model;
			model = glm::translate(model, it->second);
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}



		//glBindTexture(GL_TEXTURE_2D, transparentTexture);
		/*for (GLuint i = 0; i < vegetation.size(); i++)
		{
			glm::mat4 model;
			model = glm::translate(model, vegetation[i]);
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}*/


	}
}

int OpenglStateMultTextureTransparent::getShaderIndex()
{
	return 21;
}


bool OpenglStateMultTextureTransparent::isLihgtAction()
{
	return false;
}

int  OpenglStateMultTextureTransparent::getPointLights()
{
	return 0;
}

bool OpenglStateMultTextureTransparent::isShowLight()
{
	return false;
}


void OpenglStateMultTextureTransparent::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	//glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(normalLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoords");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

}



