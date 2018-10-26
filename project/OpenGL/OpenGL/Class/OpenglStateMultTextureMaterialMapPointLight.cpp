#include "OpenglStateMultTextureMaterialMapPointLight.h"

OpenglStateMultTextureMaterialMapPointLight::OpenglStateMultTextureMaterialMapPointLight()
{
	OpenglState::OpenglState();
}

bool OpenglStateMultTextureMaterialMapPointLight::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->genTexImage2D("resource/container.jpg", GL_RGB, 0, GL_TEXTURE0, GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/container2.png", GL_RGBA, 0, GL_TEXTURE0,GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/container2_specular.png", GL_RGBA, 0, GL_TEXTURE1, GL_REPEAT, GL_LINEAR);
	genTexImage2D("resource/matrix.jpg", GL_RGB, 0, GL_TEXTURE2, GL_REPEAT, GL_LINEAR);


	__super::initRendCommand();


	/*
		设置uniform变量有两种方法

		第一种：在initRendCommand后使用一次glUseProgram(_shaderProgram)，这个时候赋值给uniform变量
		第二种：在主循环rendeCommand里不断设置
	
	*/
	//设置纹理单元
	_glUtils->useProgram(_shaderProgram);// 先使用这个着色器程序对象才能设置uniform变量

	//通过使用glUniform1i设置采样器，我们保证了每个uniform采样器对应着正确的纹理单元
	setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0
	setInt(_shaderProgram, "material.specular", 1); // 这里的1就对应了前面的GL_TEXTURE1
	setInt(_shaderProgram, "material.emission", 2);

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

	//设置光源的颜色
	//setVec3(_shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "objectColor", 1.0f, 1.0f, 1.0f);

	//ambient材质和diffuse材质都设置成跟物体的颜色一样
	//setVec3(_shaderProgram, "material.ambient", 1.0f, 0.5f, 0.31f);
	//setVec3(_shaderProgram, "material.diffuse", 1.0f, 0.5f, 0.31f);  // 用漫反射贴图代替了
	//setVec3(_shaderProgram, "material.specular", 0.5f, 0.5f, 0.5f);  // 用高光贴图代替了
	setFloat(_shaderProgram, "material.shininess", 32.0f);


	//setVec3(_shaderProgram, "light.ambient", 0.2f, 0.2f, 0.2f);
	//setVec3(_shaderProgram, "light.diffuse", 0.5f, 0.5f, 0.5f);
	//setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);

	//设置光源的颜色
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "lightColor", lightColor);
	

	glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

	setVec3(_shaderProgram, "light.ambient", ambientColor);
	setVec3(_shaderProgram, "light.diffuse", diffuseColor);
	setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);
	//setVec3(_shaderProgram, "light.direction", -0.2f, -1.0f, -0.3f); //（0.2f, 1.0f, 0.3f) 可以理解光源的位置点

	//设置点光源的衰减变量
	setFloat(_shaderProgram, "light.constant", 1.0f);
	setFloat(_shaderProgram, "light.linear", 0.09f);
	setFloat(_shaderProgram, "light.quadratic", 0.032f);


	return true;
}
bool OpenglStateMultTextureMaterialMapPointLight::isUseEBORender()
{
	return false;
}

bool OpenglStateMultTextureMaterialMapPointLight::isLihgtAction()
{
	return true;
}

int  OpenglStateMultTextureMaterialMapPointLight::getPointLights()
{
	return 1;
}

bool OpenglStateMultTextureMaterialMapPointLight::isShowLight()
{
	return true;
}


void OpenglStateMultTextureMaterialMapPointLight::rendeCommand()
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

	
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// 光源的位置
	setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

	setVec3(_shaderProgram, "light.position", lightPos);

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)
		};
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i+1);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
}

int OpenglStateMultTextureMaterialMapPointLight::getShaderIndex()
{
	return 13;
}

void OpenglStateMultTextureMaterialMapPointLight::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//GLint colorLocation = _glUtils->getAttribLocation(_shaderProgram, "aColor");
	//glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(colorLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint normalLocation = _glUtils->getAttribLocation(_shaderProgram, "aNormal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(normalLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoords");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

}

void OpenglStateMultTextureMaterialMapPointLight::enableLightVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；
}


