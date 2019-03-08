#include "OpenglStateHDRBloom.h"

bool OpenglStateHDRBloom::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
		-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left        
	};


	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);


	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->loadTexture("resource/container.jpg",GL_TEXTURE0);
	_texture0 = loadTexture("resource/wood.png",GL_TEXTURE0);
	_texture1 = loadTexture("resource/container2.png",GL_TEXTURE0); //来回切换
	//_texture2 = loadTexture("resource/matrix.jpg", GL_TEXTURE2);

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
	//setInt(_shaderProgram, "material.specular", 1); // 这里的1就对应了前面的GL_TEXTURE1
	//setInt(_shaderProgram, "material.emission", 2);

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
	setVec3(_shaderProgram, "objectColor", lightColor);
	

	glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

	//setVec3(_shaderProgram, "light.ambient", ambientColor);
	//setVec3(_shaderProgram, "light.diffuse", diffuseColor);
	//setVec3(_shaderProgram, "light.specular", 1.0f, 1.0f, 1.0f);
	//setVec3(_shaderProgram, "light.direction", -0.2f, -1.0f, -0.3f); //（0.2f, 1.0f, 0.3f) 可以理解光源的位置点

	////设置点光源的衰减变量
	//setFloat(_shaderProgram, "light.constant", 1.0f);
	//setFloat(_shaderProgram, "light.linear", 0.09f);
	//setFloat(_shaderProgram, "light.quadratic", 0.032f);

	//定向光方向
	setVec3(_shaderProgram, "dirLight.direction", -0.2f, -1.0f, -0.3f); //（0.2f, 1.0f, 0.3f) 可以理解光源的位置点
	setVec3(_shaderProgram, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
	setVec3(_shaderProgram, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	setVec3(_shaderProgram, "dirLight.specular", 0.5f, 0.5f, 0.5f);

	
	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(0.0f, 0.5f, 1.5f),
	//	glm::vec3(-4.0f, 0.5f, -3.0f),
	//	glm::vec3(3.0f, 0.5f, 1.0f),
	//	glm::vec3(-.8f, 2.4f, -1.0f)
	//};


	//glm::vec3 pointLightColors[] = {
	//	glm::vec3(2.0f, 2.0f, 2.0f),
	//	glm::vec3(1.5f, 0.0f, 0.0f),
	//	glm::vec3(0.0f, 0.0f, 1.5f),
	//	glm::vec3(0.0f, 1.5f, 0.0f)
	//};


	//点光源
	_lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	_lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	_lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	_lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));


	_lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	_lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	_lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	_lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));


	_lightScale.push_back(glm::vec3(0.5));
	_lightScale.push_back(glm::vec3(0.5));
	_lightScale.push_back(glm::vec3(0.5));
	_lightScale.push_back(glm::vec3(0.5));


	std::string s = "pointLights";
	for (unsigned int i = 0; i < 4; i++)
	{
		std::string positon = s;
		positon = positon + "[" + to_string(i) + "].position";


		std::string color = s;
		color = color + "[" + to_string(i) + "].color";

		std::string ambient = s;
		ambient = ambient + "[" + to_string(i) + "].ambient";

		std::string diffuse = s;
		diffuse = diffuse + "[" + to_string(i) + "].diffuse";

		std::string specular = s;
		specular = specular + "[" + to_string(i) + "].specular";

		std::string constant = s;
		constant = constant + "[" + to_string(i) + "].constant";

		std::string linear = s;
		linear = linear + "[" + to_string(i) + "].linear";

		std::string quadratic = s;
		quadratic = quadratic + "[" + to_string(i) + "].quadratic";

		setVec3(_shaderProgram, positon.c_str(), _lightPositions[i]);
		setVec3(_shaderProgram, color.c_str(), _lightColors[i]);

		//设置光源分量的不同强度
		setVec3(_shaderProgram, ambient.c_str(), 0.05f, 0.05f, 0.05f);
		setVec3(_shaderProgram, diffuse.c_str(), 0.8f, 0.8f, 0.8f);
		setVec3(_shaderProgram, specular.c_str(), 1.0f, 1.0f, 1.0f);

		//设置点光源的衰减变量
		setFloat(_shaderProgram, constant.c_str(), 1.0f);
		setFloat(_shaderProgram, linear.c_str(), 0.09);
		setFloat(_shaderProgram, quadratic.c_str(), 0.032);
	}
	

	//聚光
	setVec3(_shaderProgram, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
	setVec3(_shaderProgram, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "spotLight.specular", 1.0f, 1.0f, 1.0f);
	setFloat(_shaderProgram, "spotLight.constant", 1.0f);
	setFloat(_shaderProgram, "spotLight.linear", 0.09);
	setFloat(_shaderProgram, "spotLight.quadratic", 0.032);
	setFloat(_shaderProgram, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	setFloat(_shaderProgram, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	setInt(_shaderProgram, "inverse_normals", true);



	//箱子的数据

	cubePosition.push_back(glm::vec3(0.0f, 1.5f, 0.0));
	cubePosition.push_back(glm::vec3(2.0f, 0.0f, 1.0));
	cubePosition.push_back(glm::vec3(-1.0f, -1.0f, 2.0));
	cubePosition.push_back(glm::vec3(0.0f, 2.7f, 4.0));
	cubePosition.push_back(glm::vec3(-2.0f, 1.0f, -3.0));
	cubePosition.push_back(glm::vec3(-3.0f, 0.0f, 0.0));

	cubeScale.push_back(glm::vec3(0.5f));
	cubeScale.push_back(glm::vec3(0.5f));
	cubeScale.push_back(glm::vec3(1.0f));
	cubeScale.push_back(glm::vec3(1.25f));
	cubeScale.push_back(glm::vec3(1.0f));
	cubeScale.push_back(glm::vec3(0.5f));

	cubeRotate.push_back(0.0f);
	cubeRotate.push_back(0.0f);
	cubeRotate.push_back(60.0f);
	cubeRotate.push_back(23.0f);
	cubeRotate.push_back(124.0f);
	cubeRotate.push_back(0.0f);



	// ping-pong-framebuffer for blurring
	//unsigned int pingpongFBO[2];
	//unsigned int pingpongColorbuffers[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}


	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/blur.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/blur.frag");
	_glUtils->linkShader(&_shaderBlurProgram, _vertexShader, _fragmentShader);

	return true;
}
bool OpenglStateHDRBloom::isUseEBORender()
{
	return false;
}

bool OpenglStateHDRBloom::isUseFrameBuffer()
{
	return true;
}

bool OpenglStateHDRBloom::isUseHDR()
{
	return true;
}

float OpenglStateHDRBloom::getExposure()
{
	return 3.0f;
}

int  OpenglStateHDRBloom::getPointLights()
{
	return 4;
}

bool OpenglStateHDRBloom::isShowLight()
{
	return true;
}

bool OpenglStateHDRBloom::isUseCustomLightPos()
{
	return true;
}

bool OpenglStateHDRBloom::isUseBloom()
{
	return true;
}


void OpenglStateHDRBloom::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();
	setFloat(_shaderProgram, "textureAlpha", _param1);

	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0

	//activiteTexture(GL_TEXTURE1);
	//bindTexture(_texture1);
	//setInt(_shaderProgram, "material.specular", 1); // 这里的1就对应了前面的GL_TEXTURE1

	//activiteTexture(GL_TEXTURE2);
	//bindTexture(_texture2);
	//setInt(_shaderProgram, "material.emission", 2); // 这里的1就对应了前面的GL_TEXTURE2

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2 + glm::vec3(0,1,0);
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);	

	
	//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	//setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// 光源的位置
	//setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

	//setVec3(_shaderProgram, "light.position", lightPos);

	//聚光配置
	setVec3(_shaderProgram, "spotLight.position", cameraPos);
	setVec3(_shaderProgram, "spotLight.direction", cameraFront);



	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		setMat4(_shaderProgram, "model", &model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/*glm::vec3 cubePositions[] = {
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
		}*/
	}

	//同一个shader 不同texture切换
	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture1);
	//setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0

	int count = cubePosition.size();
	for (int i = 0; i < count; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePosition[i]);
		model = glm::rotate(model, glm::radians(cubeRotate[i]), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, cubeScale[i]);
		setMat4(_shaderProgram, "model", &model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 2. blur bright fragments with two-pass Gaussian Blur 
	// --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	glUseProgram(_shaderBlurProgram);
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
		setInt(_shaderBlurProgram, "horizontal", horizontal);

		//colorBuffers[1]是亮度区域的纹理
		glBindTexture(GL_TEXTURE_2D, first_iteration ? _brightTexture : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	//模糊过的亮度区域纹理
	_blurTexture = pingpongColorbuffers[!horizontal];
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

int OpenglStateHDRBloom::getShaderIndex()
{
	return 35;
}

void OpenglStateHDRBloom
::enableVertexAttribArray()
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



