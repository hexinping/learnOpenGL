#include "OpenglStateMultTextureLight.h"

OpenglStateMultTextureLight::OpenglStateMultTextureLight()
{
	OpenglState::OpenglState();
}

bool OpenglStateMultTextureLight::init(string vertFile, string fragFile)
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	float lightVertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	int len1 = sizeof(vertices) / sizeof(float);
	int len2 = sizeof(lightVertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);
	memcpy(_lightVertices, lightVertices, sizeof(float) * len2);

	_vertFile = vertFile;
	_fragFile = fragFile;

	this->genTexImage2D("resource/container.jpg", GL_RGB, 0, GL_TEXTURE0, GL_REPEAT, GL_LINEAR);
	this->genTexImage2D("resource/awesomeface.png", GL_RGBA, 0, GL_TEXTURE1,GL_REPEAT, GL_LINEAR);

	setLight(true);

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
	setInt(_shaderProgram,"texture2", 1); // 这里的1就对应了前面的GL_TEXTURE1

	//glm::mat4 trans;
	//float angle = 90.0f;
	//glm::vec3 axis(0.0, 0.0, 1.0);
	//glm::vec3 s(0.5, 0.5, 0.5);
	//
	//_mathUtils->setRoateMat4(&trans, angle, &axis);
	//_mathUtils->setSclaeMat4(&trans, &s);

	////先缩放 再旋转 ==》与设置顺序相反
	//setMat4(_shaderProgram, "transform", &trans);

	//glm::mat4 model;
	//glm::vec3 asix(1.0f, 0.0f, 0.0f); //绕X轴旋转
	////90.0f 逆时针
	//_mathUtils->setRotateMat4(&model, -55.0f, &asix);

	

	//float width = 800.0f;
	//float height = 600.0f;
	//glm::mat4 projection;
	//_mathUtils->setPerspective(&projection, 45.0f, width, height, 0.1f, 100.0f);

	//setMat4(_shaderProgram, "model", &model);
	//setMat4(_shaderProgram, "view", &view);
	//setMat4(_shaderProgram, "projection", &projection);



	return true;
}
bool OpenglStateMultTextureLight::isUseEBORender()
{
	return false;
}

void OpenglStateMultTextureLight::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();
	setFloat(_shaderProgram, "textureAlpha", _param1);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);	

	//设置光源的颜色
	setVec3(_shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);


	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
		};
		for (unsigned int i = 0; i < 1; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i+1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//光源的位置
		if (_isLight)
		{	
			__super::lightRendeCommand();
			

			glm::mat4 model;
			glm::vec3 lightPos(0.3f, 0.4f, 2.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.05f));
			setMat4(_lightShaderProgram, "model", &model);

			setMat4(_lightShaderProgram, "view", &view);
			setMat4(_lightShaderProgram, "projection", &projection);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

int OpenglStateMultTextureLight::getShaderIndex()
{
	return 10;
}

void OpenglStateMultTextureLight::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

}

void OpenglStateMultTextureLight::enableLightVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}


