#include "OpenglStateMultTextureBlend.h"
#include <vector>
bool OpenglStateMultTextureBlend::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
		1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 0.0f, 1.0f, 0.0f
	};



	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->loadTexture("resource/grass.png", GL_TEXTURE0);
	_texture0 = loadTexture("resource/mode1.png", GL_TEXTURE0);
	_texture1 = loadTexture("resource/noise.jpg", GL_TEXTURE1);
	_texture2 = loadTexture("resource/water.jpg", GL_TEXTURE1);

	__super::initRendCommand();


	/*
		设置uniform变量有两种方法

		第一种：在initRendCommand后使用一次glUseProgram(_shaderProgram)，这个时候赋值给uniform变量
		第二种：在主循环rendeCommand里不断设置
	
	*/
	//设置纹理单元
	_glUtils->useProgram(_shaderProgram);// 先使用这个着色器程序对象才能设置uniform变量


	//通过使用glUniform1i设置采样器，我们保证了每个uniform采样器对应着正确的纹理单元
	setInt(_shaderProgram, "texture1", 0); // 这里的0就对应了前面的GL_TEXTURE0





	// transparent VAO
	
	//glGenVertexArrays(1, &transparentVAO);
	//glGenBuffers(1, &transparentVBO);
	//glBindVertexArray(transparentVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glBindVertexArray(0);

	//提取亮度
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/extractBright.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/extractBright.frag");
	_glUtils->linkShader(&_shaderBrightProgram, _vertexShader, _fragmentShader);

	//水平模糊
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/blue_h.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/blue_h.frag");
	_glUtils->linkShader(&_shaderBlurHProgram, _vertexShader, _fragmentShader);

	////垂直模糊
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/blue_v.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/blue_v.frag");
	_glUtils->linkShader(&_shaderBlurVProgram, _vertexShader, _fragmentShader);

	return true;
}


bool OpenglStateMultTextureBlend::isUseEBORender()
{
	return false;
}

bool  OpenglStateMultTextureBlend::isUsePlane()
{
	return true;
}

void OpenglStateMultTextureBlend::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();
	
	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "texture1", 0);

	activiteTexture(GL_TEXTURE1);
	bindTexture(_texture1);
	setInt(_shaderProgram, "NoiseTexture", 1);

	activiteTexture(GL_TEXTURE2);
	bindTexture(_texture2);
	setInt(_shaderProgram, "uvTexture", 2);

	setFloat(_shaderProgram, "Time", (float)glfwGetTime());

	setFloat(_shaderProgram, "dissolveFactor", 0.4);
	setFloat(_shaderProgram, "edgeWidth", 0.1);

	setVec2(_shaderProgram, "resolution", 800.0f, 600.0f);
	setVec2(_shaderProgram, "textureSize", 523, 418);// 暂时写死

	//NoiseTexture

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

		//glBindTexture(GL_TEXTURE_2D, transparentTexture);
		for (GLuint i = 0; i < vegetation.size(); i++)
		{
			glm::mat4 model;
			model = glm::translate(model, vegetation[i]);
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


	}
}

int OpenglStateMultTextureBlend::getShaderIndex()
{
	return 20;
}


bool OpenglStateMultTextureBlend::isLihgtAction()
{
	return false;
}

int  OpenglStateMultTextureBlend::getPointLights()
{
	return 0;
}

bool OpenglStateMultTextureBlend::isShowLight()
{
	return false;
}


void OpenglStateMultTextureBlend::enableVertexAttribArray()
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



