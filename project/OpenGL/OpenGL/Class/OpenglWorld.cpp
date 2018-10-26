#include "OpenglWorld.h"

unsigned int _lightVBO, _lightVAO, _lightEBO;
unsigned int _lightShaderProgram;
OpenglWorld::OpenglWorld()
:_isLight(false)
,_glUtils(nullptr)
,_mathUtils(nullptr)
, _lightVBO(0)
, _lightVAO(0)
, _lightEBO(0)
, _lightShaderProgram(0)
{
	_isLight = false;
	_glUtils = OpenglUtils::getInstance();
	_mathUtils = MathUtils::getInstance();
}

void  OpenglWorld::init()
{

	if (_isLight)
	{
		float lightVertices[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
		};

		
		int len2 = sizeof(lightVertices) / sizeof(float);
		memcpy(_lightVertices, lightVertices, sizeof(float) * len2);


		unsigned int _lightVertexShader;
		unsigned int _lightFragmentShader;
		_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_lightVertexShader, "shader/lamp.vert");
		_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_lightFragmentShader, "shader/lamp.frag");
		_glUtils->linkShader(&_lightShaderProgram, _lightVertexShader, _lightFragmentShader);

		_glUtils->bindVBOAndVAO(&_lightVBO, &_lightVAO, _lightVertices, sizeof(_lightVertices), false, &_lightEBO, _lightIndices, sizeof(_lightIndices));
		GLint posLocation = _glUtils->getAttribLocation(_lightShaderProgram, "aPos");
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
		glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
	}
}

void OpenglWorld::add(OpenglState *glState)
{
	_openglStateArray.push_back(glState);
}

void OpenglWorld::setLight(bool light)
{
	_isLight = light;
}

void OpenglWorld::setLightNum(int num)
{
	_lightNums = num;
}

void OpenglWorld::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	glUseProgram(_lightShaderProgram);
	glBindVertexArray(_lightVAO);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
	

	glm::mat4 view;
	glm::vec3 cameraPos = _param2;
	glm::vec3 cameraFront = _param3;
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	if (_islightAction)
	{
		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	}
	
	
	//光源的位置
	if (_isLight)
	{

		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f, 0.2f, 2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f, 2.0f, -12.0f),
			glm::vec3(0.0f, 0.0f, -3.0f)
		};

		for (unsigned int i = 0; i < _lightNums; i++)
		{
			glm::mat4 model;
			if (!_islightAction)
			{
				model = glm::translate(model, pointLightPositions[i]);
			}
			else
			{
				model = glm::translate(model, lightPos);
			}
			model = glm::scale(model, glm::vec3(0.5f));
			_glUtils->setMat4(_lightShaderProgram, "model", &model);
			_glUtils->setMat4(_lightShaderProgram, "view", &view);
			_glUtils->setMat4(_lightShaderProgram, "projection", &projection);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
	}
}

void OpenglWorld::setLightAction(bool isAction)
{
	_islightAction = isAction;
}

void OpenglWorld::setRenderModel(bool isRenderModel)
{
	_isRenderModel = isRenderModel;
}

void OpenglWorld::setModelOpenglState(OpenglState *glState)
{
	_modleState = glState;
}

OpenglWorld::~OpenglWorld()
{
	int size = _openglStateArray.size();
	for (int i = 0; i < size; i++)
	{
		OpenglState * glState = _openglStateArray[i];
		delete glState;
	}
	_openglStateArray.clear();
}



