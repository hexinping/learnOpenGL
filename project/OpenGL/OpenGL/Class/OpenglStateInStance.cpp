#include "OpenglStateInStance.h"
#include <vector>
bool OpenglStateInStance::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// 位置          // 颜色
		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

		-0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
		0.05f, 0.05f, 0.0f, 1.0f, 1.0f
	};



	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->loadTexture("resource/container.jpg", GL_TEXTURE0);
	//_texture0 = loadTexture("resource/grass.png",GL_TEXTURE0);

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


	

	//for (unsigned int i = 0; i < 100; i++)
	//{
	//	string index = to_string(i);
	//	setVec2(_shaderProgram,("offsets[" + index + "]").c_str(), translations[i]);
	//}

	return true;
}
bool OpenglStateInStance::isUseEBORender()
{
	return false;
}

bool  OpenglStateInStance::isUsePlane()
{
	return false;
}

void OpenglStateInStance::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();

	//glDisable(GL_DEPTH_TEST);
	
	//activiteTexture(GL_TEXTURE0);
	//bindTexture(_texture0);
	//setInt(_shaderProgram, "texture1", 0);

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


		//vector<glm::vec3> vegetation
		//{
		//	glm::vec3(-1.5f, 0.0f, -0.48f),
		//	glm::vec3(1.5f, 0.0f, 0.51f),
		//	glm::vec3(0.0f, 0.0f, 0.7f),
		//	glm::vec3(-0.3f, 0.0f, -2.3f),
		//	glm::vec3(0.5f, 0.0f, -0.6f)
		//};

		////glBindTexture(GL_TEXTURE_2D, transparentTexture);
		//for (GLuint i = 0; i < vegetation.size(); i++)
		//{
		//	glm::mat4 model;
		//	model = glm::translate(model, vegetation[i]);
		//	setMat4(_shaderProgram, "model", &model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 400); //替代100次循环，减少cpu和gpu的通信，提升效率
	
	}
}

int OpenglStateInStance::getShaderIndex()
{
	return 25;
}


bool OpenglStateInStance::isLihgtAction()
{
	return false;
}

int  OpenglStateInStance::getPointLights()
{
	return 0;
}

bool OpenglStateInStance::isShowLight()
{
	return false;
}


void OpenglStateInStance::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	//glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(normalLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的



	//实例化数组:没有上限的设置
	glm::vec2 translations[400];
	int index = 0;
	float offset = 0.5f;
	for (int y = -10; y < 10; y += 1)
	{
		for (int x = -10; x < 10; x += 1)
		{
			glm::vec2 translation;
			translation.x = (float)x / 5.0f + offset;
			translation.y = (float)y / 5.0f + offset;
			translations[index++] = translation;
		}
	}

	// store instance data in an array buffer
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 400, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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



