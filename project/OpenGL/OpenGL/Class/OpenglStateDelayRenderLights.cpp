#include "OpenglStateDelayRenderLights.h"

bool OpenglStateDelayRenderLights::init(string vertFile, string fragFile)
{

	_vertFile = vertFile;
	_fragFile = fragFile;

	//this->loadTexture("resource/container.jpg",GL_TEXTURE0);
	_texture0 = loadTexture("resource/wood.png",GL_TEXTURE0);
	//_texture1 = loadTexture("resource/container2.png",GL_TEXTURE0); //来回切换
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


	


	//设置光源的颜色
	//setVec3(_shaderProgram, "lightColor", 1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "objectColor", 1.0f, 1.0f, 1.0f);

	//ambient材质和diffuse材质都设置成跟物体的颜色一样
	//setVec3(_shaderProgram, "material.ambient", 1.0f, 0.5f, 0.31f);
	//setVec3(_shaderProgram, "material.diffuse", 1.0f, 0.5f, 0.31f);  // 用漫反射贴图代替了
	//setVec3(_shaderProgram, "material.specular", 0.5f, 0.5f, 0.5f);  // 用高光贴图代替了
	setFloat(_shaderProgram, "material.shininess", 32.0f);



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

	

	////点光源 放到光照延迟渲染里了
	//_lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	//_lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	//_lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	//_lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));


	//_lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	//_lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	//_lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	//_lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));


	//_lightScale.push_back(glm::vec3(0.5));
	//_lightScale.push_back(glm::vec3(0.5));
	//_lightScale.push_back(glm::vec3(0.5));
	//_lightScale.push_back(glm::vec3(0.5));


	//std::string s = "pointLights";
	//for (unsigned int i = 0; i < 4; i++)
	//{
	//	std::string positon = s;
	//	positon = positon + "[" + to_string(i) + "].position";


	//	std::string color = s;
	//	color = color + "[" + to_string(i) + "].color";

	//	std::string ambient = s;
	//	ambient = ambient + "[" + to_string(i) + "].ambient";

	//	std::string diffuse = s;
	//	diffuse = diffuse + "[" + to_string(i) + "].diffuse";

	//	std::string specular = s;
	//	specular = specular + "[" + to_string(i) + "].specular";

	//	std::string constant = s;
	//	constant = constant + "[" + to_string(i) + "].constant";

	//	std::string linear = s;
	//	linear = linear + "[" + to_string(i) + "].linear";

	//	std::string quadratic = s;
	//	quadratic = quadratic + "[" + to_string(i) + "].quadratic";

	//	setVec3(_shaderProgram, positon.c_str(), _lightPositions[i]);
	//	setVec3(_shaderProgram, color.c_str(), _lightColors[i]);

	//	//设置光源分量的不同强度
	//	setVec3(_shaderProgram, ambient.c_str(), 0.05f, 0.05f, 0.05f);
	//	setVec3(_shaderProgram, diffuse.c_str(), 0.8f, 0.8f, 0.8f);
	//	setVec3(_shaderProgram, specular.c_str(), 1.0f, 1.0f, 1.0f);

	//	//设置点光源的衰减变量
	//	setFloat(_shaderProgram, constant.c_str(), 1.0f);
	//	setFloat(_shaderProgram, linear.c_str(), 0.09);
	//	setFloat(_shaderProgram, quadratic.c_str(), 0.032);
	//}
	


	std::vector<glm::vec3> objectPositions;
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));


	unsigned int amount = 9;
	_amount = amount;
	//glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, objectPositions[i]);

		//// 2. 缩放：在 0.05 和 0.25f 之间缩放
		//float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(0.25f));

		//// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		//float rotAngle = (rand() % 360);
		//model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}




	return true;
}
bool OpenglStateDelayRenderLights::isUseEBORender()
{
	return false;
}

bool OpenglStateDelayRenderLights::isUseFrameBuffer()
{
	return true;
}


int  OpenglStateDelayRenderLights::getPointLights()
{
	return 4;
}

bool OpenglStateDelayRenderLights::isShowLight()
{
	return false;
}

bool OpenglStateDelayRenderLights::isUseCustomLightPos()
{
	return true;
}



string OpenglStateDelayRenderLights::getModelFile()
{
	string str("resource/objects/nanosuit/nanosuit.obj");
	return str;
}

bool OpenglStateDelayRenderLights::isRenderModel()
{
	return true;
}

bool OpenglStateDelayRenderLights::isDelayRenderLights()
{
	return true;
}

void OpenglStateDelayRenderLights::rendeCommand()
{
	_glUtils->useProgram(_shaderProgram);

	//activiteTexture(GL_TEXTURE4);
	//bindTexture(_cubemapTexture, true);
	//setInt(_shaderProgram, "skybox", 4); // 这里的0就对应了前面的GL_TEXTURE0

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	setMat4(_shaderProgram, "projection", &projection);

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	setMat4(_shaderProgram, "view", &view);

	//cout << "(float)glfwGetTime():" << (float)glfwGetTime() << endl;

	// render the loaded model
	if (!_useModelMat4)
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		setMat4(_shaderProgram, "model", &model);
	}
	else
	{
		setMat4(_shaderProgram, "model", &_modelMat4);
	}


	setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）



	//if (_isUseEBORender)
	//{
	//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	//}
	//else
	//{
	//	glm::mat4 model;
	//	model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
	//	model = glm::rotate(model,  glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
	//	setMat4(_shaderProgram, "model", &model);
	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//	/*glm::vec3 cubePositions[] = {
	//		glm::vec3(0.0f, 0.0f, 0.0f),
	//		glm::vec3(2.0f, 5.0f, -15.0f),
	//		glm::vec3(-1.5f, -2.2f, -2.5f),
	//		glm::vec3(-3.8f, -2.0f, -12.3f),
	//		glm::vec3(2.4f, -0.4f, -3.5f),
	//		glm::vec3(-1.7f, 3.0f, -7.5f),
	//		glm::vec3(1.3f, -2.0f, -2.5f),
	//		glm::vec3(1.5f, 2.0f, -2.5f),
	//		glm::vec3(1.5f, 0.2f, -1.5f),
	//		glm::vec3(-1.3f, 1.0f, -1.5f)
	//	};
	//	for (unsigned int i = 0; i < 10; i++)
	//	{
	//		glm::mat4 model;
	//		model = glm::translate(model, cubePositions[i]);
	//		float angle = 20.0f * (i+1);
	//		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//		setMat4(_shaderProgram, "model", &model);
	//		glDrawArrays(GL_TRIANGLES, 0, 36);
	//	}*/
	//}

	

}

int OpenglStateDelayRenderLights::getShaderIndex()
{
	return 36;
}



