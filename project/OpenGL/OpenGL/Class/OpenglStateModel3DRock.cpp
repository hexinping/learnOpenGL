#include "OpenglStateModel3DRock.h"

bool OpenglStateModel3DRock::init(string vertFile, string fragFile)
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


	unsigned int amount = 1000;
	_amount = amount;
	//glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // 初始化随机种子    
	float radius = 25.0f;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. 缩放：在 0.05 和 0.25f 之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}


	return true;
}

int OpenglStateModel3DRock::getShaderIndex()
{
	return 27;
}

bool OpenglStateModel3DRock::isRenderModel()
{
	return true;
}

string OpenglStateModel3DRock::getModelFile()
{
	string str("resource/objects/rock/rock.obj");
	return str;
}

bool OpenglStateModel3DRock::isUseReflect()
{
	return false;
}

bool OpenglStateModel3DRock::isUseSkyBox()
{
	return false;
}

void OpenglStateModel3DRock::rendeCommand()
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
	//for (unsigned int i = 0; i < _amount; i++)
	//{
	//	glm::mat4 model = modelMatrices[i];
	//	setMat4(_shaderProgram, "model", &model);
	//	
	//}
	

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


OpenglStateModel3DRock::~OpenglStateModel3DRock()
{
	delete []modelMatrices;
}

