#include "OpenglStateInstance2.h"
#include <vector>
bool OpenglStateInstance2::init(string vertFile, string fragFile)
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
	_texture0 = genTexImage2D("resource/grass.png", GL_RGBA, 0, GL_TEXTURE0, GL_CLAMP_TO_EDGE, GL_LINEAR);

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

	

	return true;
}
bool OpenglStateInstance2::isUseEBORender()
{
	return false;
}

bool  OpenglStateInstance2::isUsePlane()
{
	return false;
}

void OpenglStateInstance2::rendeCommand()
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

	
	glm::mat4 model;
	setMat4(_shaderProgram, "model", &model);

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		// 1使用glDrawArrays渲染，要调用400次，cpu和gpu通信代价太大

		//vector<glm::vec3> vegetation;
		//float offset = 0.5f;
		//for (int y = -10; y < 10; y += 1)
		//{
		//	for (int x = -10; x < 10; x += 1)
		//	{
		//		glm::vec3 translation;
		//		translation.x = (float)x  + offset;
		//		translation.y = (float)y  + offset;
		//		translation.z = 0.0f;
		//		vegetation.push_back(translation);
		//	}
		//}

		//for (GLuint i = 0; i < vegetation.size(); i++)
		//{
		//	glm::mat4 model;
		//	model = glm::translate(model, vegetation[i]);
		//	setMat4(_shaderProgram, "model", &model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		// 2使用glDrawArraysInstanced渲染，要调用1次，cpu和gpu通信代价明显降低
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 400); //替代400次循环，减少cpu和gpu的通信，提升效率


	}
}

int OpenglStateInstance2::getShaderIndex()
{
	return 26;
}


bool OpenglStateInstance2::isLihgtAction()
{
	return false;
}

int  OpenglStateInstance2::getPointLights()
{
	return 0;
}

bool OpenglStateInstance2::isShowLight()
{
	return false;
}


void OpenglStateInstance2::enableVertexAttribArray()
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

	//实例化数组:没有上限的设置
	glm::vec2 translations[400];
	int index = 0;
	float offset = 0.5f;
	for (int y = -10; y < 10; y += 1)
	{
		for (int x = -10; x < 10; x += 1)
		{
			glm::vec2 translation;
			translation.x = (float)x  + offset;
			translation.y = (float)y + offset;
			translations[index++] = translation;
		}
	}

	// store instance data in an array buffer
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 400, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 还可以自己构造属性传给shader，
	GLint aOffsetLocation = _glUtils->getAttribLocation(_shaderProgram, "aOffset");
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(aOffsetLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(aOffsetLocation);
	glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

	/*
	glVertexAttribDivisor。这个函数告诉了OpenGL该什么时候更新顶点属性的内容至新一组数据。
	它的第一个参数是需要的顶点属性，第二个参数是属性除数(Attribute Divisor)。
	默认情况下，属性除数是0，告诉OpenGL我们需要在顶点着色器的每次迭代时更新顶点属性。
	将它设置为1时，我们告诉OpenGL我们希望在渲染一个新实例的时候更新顶点属性。而设置为2时，我们希望每2个实例更新一次属性
	*/

}


