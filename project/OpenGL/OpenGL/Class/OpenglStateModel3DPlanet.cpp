#include "OpenglStateModel3DPlanet.h"

bool OpenglStateModel3DPlanet::init(string vertFile, string fragFile)
{
	_vertFile = vertFile;
	_fragFile = fragFile;
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, _vertFile);
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, _fragFile);
	_glUtils->linkShader(&_shaderProgram, _vertexShader, _fragmentShader);

	_glUtils->useProgram(_shaderProgram);

	//设置光源的颜色
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	setVec3(_shaderProgram, "lightColor", lightColor);
	setVec3(_shaderProgram, "objectColor", lightColor);

	setFloat(_shaderProgram, "material.shininess", 32.0f);


	//点光源
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	std::string s = "pointLights";
	for (unsigned int i = 0; i < 4; i++)
	{
		std::string positon = s;
		positon = positon + "[" + to_string(i) + "].position";

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

		setVec3(_shaderProgram, positon.c_str(), pointLightPositions[i]);

		//设置光源分量的不同强度
		setVec3(_shaderProgram, ambient.c_str(), 0.05f, 0.05f, 0.05f);
		setVec3(_shaderProgram, diffuse.c_str(), 0.8f, 0.8f, 0.8f);
		setVec3(_shaderProgram, specular.c_str(), 1.0f, 1.0f, 1.0f);

		//设置点光源的衰减变量
		setFloat(_shaderProgram, constant.c_str(), 1.0f);
		setFloat(_shaderProgram, linear.c_str(), 0.09);
		setFloat(_shaderProgram, quadratic.c_str(), 0.032);
	}
	return true;
}

int OpenglStateModel3DPlanet::getShaderIndex()
{
	return 27;
}

bool OpenglStateModel3DPlanet::isRenderModel()
{
	return true;
}

string OpenglStateModel3DPlanet::getModelFile()
{
	string str("resource/objects/planet/planet.obj");
	return str;
}

bool OpenglStateModel3DPlanet::isUseReflect()
{
	return false;
}

bool OpenglStateModel3DPlanet::isUseSkyBox()
{
	return false;
}

void OpenglStateModel3DPlanet::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	
	_glUtils->useProgram(_shaderProgram);

	activiteTexture(GL_TEXTURE4);
	bindTexture(_cubemapTexture, true);
	setInt(_shaderProgram, "skybox", 4); // 这里的0就对应了前面的GL_TEXTURE0

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
	//绘制行星
	if (!_useModelMat4)
	{
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));	// it's a bit too big for our scene, so scale it down
		setMat4(_shaderProgram, "model", &model);
	}
	else
	{
		setMat4(_shaderProgram, "model", &_modelMat4);
	}
	

	setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(0.7f, 0.2f, 2.0f),
	//	glm::vec3(2.3f, -3.3f, -4.0f),
	//	glm::vec3(-4.0f, 2.0f, -12.0f),
	//	glm::vec3(0.0f, 0.0f, -3.0f)
	//};
	//for (unsigned int i = 0; i < 1; i++)
	//{
	//	glm::mat4 model;
	//	model = glm::translate(model, pointLightPositions[i]);
	//	//model = glm::scale(model, glm::vec3(0.05f));
	//	setMat4(_lightShaderProgram, "model", &model);

	//	setMat4(_lightShaderProgram, "view", &view);
	//	setMat4(_lightShaderProgram, "projection", &projection);

	//	glDrawArrays(GL_TRIANGLES, 0, 36);
	//}

}

