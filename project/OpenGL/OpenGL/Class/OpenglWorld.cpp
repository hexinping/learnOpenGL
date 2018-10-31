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

void OpenglWorld::add(OpenglState *glState, bool isFront)
{
	if (isFront)
	{
		_openglStateArray.insert(_openglStateArray.begin(), glState);
		return;
	}
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


void OpenglWorld::setUseFameBuffer(bool isUse)
{
	_isUseFrameBuffer = isUse;
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

void OpenglWorld::addModelOpenglState(OpenglState *glState, int id)
{
	_creaetModelMap.insert(pair<int, OpenglState*>(id, glState));
}

void OpenglWorld::addRenderModel(Model *model, int id)
{
	_renderModelMap.insert(pair<int, Model*>(id, model));
}

void OpenglWorld::createFrameBuffer(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *texColorBuffer)
{
	//自定义一个帧缓冲对象
	
	glGenFramebuffers(1, framebuffer);					//创建一个帧缓冲对象
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);     //把创建的帧缓冲对象绑定到GL_FRAMEBUFFER上

	// 生成纹理附件
	glGenTextures(1, texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, *texColorBuffer);

	// 这里把纹理大小设置为屏幕大小，
	//data用null表示仅仅分配了内存但没有填充纹理，填充纹理放到渲染帧缓冲对象之后
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 将纹理附件附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texColorBuffer, 0);


	//创建一个渲染缓冲对象(用于深度和模板缓冲，只读)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); //内部格式设置为GL_DEPTH24_STENCIL8
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//将渲染缓冲对象附加到帧缓冲的深度和模板附件上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //把创建的帧缓冲对象输出到屏幕，必须要有这句
}

void OpenglWorld::createFrameBufferByMultSample(int screenWidth, int screenHeight, unsigned int *framebuffer, unsigned int *texColorBuffer)
{
	
	glGenFramebuffers(1, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
	// create a multisampled color attachment texture
	unsigned int textureColorBufferMultiSampled;
	glGenTextures(1, &textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled); //绑定多重采样GL_TEXTURE_2D_MULTISAMPLE
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, screenWidth, screenHeight, GL_TRUE);//生成多重采样纹理
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	//将多重采样纹理附加到帧缓冲上 类型是GL_TEXTURE_2D_MULTISAMPLE
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	// create a (also multisampled) renderbuffer object for depth and stencil attachments
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	//多重采样渲染缓冲对象
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); //4代表样本数量
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenglWorld::useFrameBuffer(unsigned int framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
}

void OpenglWorld::deleteBuffers(unsigned int framebuffer)
{
	glDeleteBuffers(1, &_lightEBO);
	glDeleteVertexArrays(1, &_lightVAO);
	glDeleteBuffers(1, &_lightVBO);

	if (_isUseFrameBuffer)
	{
		//删除帧缓冲对象
		glDeleteFramebuffers(1, &framebuffer);
	}
}

void OpenglWorld::setCubemapTexture(unsigned int cubemapTexture)
{
	_cubemapTexture = cubemapTexture;
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


	map<int, Model *>::iterator iter;
	for (iter = _renderModelMap.begin(); iter != _renderModelMap.end(); iter++)
	{
		Model *model = iter->second;
		delete model;
	}
	_creaetModelMap.clear();
	_renderModelMap.clear();
}



