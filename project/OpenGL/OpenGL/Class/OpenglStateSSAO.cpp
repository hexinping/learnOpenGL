#include "OpenglStateSSAO.h"
#include <random>


/*

	思路：利用多个帧缓冲处理数据以及传递计算结果

	1 world创建G缓冲，glstate 默认shader创建处理数据
	G缓冲==》数据采集

	2 glstate创建多个帧缓冲和shader进行传递以及计算
	SSAO缓冲 ==》计算遮蔽因子
	模糊缓冲 ==》模糊效果
	默认帧缓冲 ==》计算最后光照效果

*/
float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

bool OpenglStateSSAO::init(string vertFile, string fragFile)
{

	_vertFile = vertFile;
	_fragFile = fragFile;

	__super::initRendCommand();


	lightPos = glm::vec3(2.0, 4.0, -2.0);
	lightColor = glm::vec3(0.2, 0.2, 0.7);

	unsigned int amount = 1;
	_amount = amount;
	//glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));

		// 4. 添加到矩阵的数组中
		modelMatrices[i] = model;
	}



	//使用G缓冲中采样数据 ssao着色器 计算遮蔽因子
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/ssao.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/ssao.frag");
	_glUtils->linkShader(&_shaderSSAO, _vertexShader, _fragmentShader);


	//模糊
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/ssao.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/ssaoBlur.frag");
	_glUtils->linkShader(&_shaderSSAOBlur, _vertexShader, _fragmentShader);


	//光照计算着色器
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/ssao.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/ssaoLighting.frag");
	_glUtils->linkShader(&_shaderLightingPass, _vertexShader, _fragmentShader);


	// also create framebuffer to hold SSAO processing stage 
	// -----------------------------------------------------
	//计算遮蔽因子的帧缓冲
	glGenFramebuffers(1, &ssaoFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	unsigned int ssaoColorBuffer;
	// SSAO color buffer
	glGenTextures(1, &ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	//由于环境遮蔽的结果是一个灰度值，我们将只需要纹理的红色分量，所以我们将颜色缓冲的内部格式设置为GL_RED。
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 模糊帧缓冲
	unsigned int ssaoColorBufferBlur;
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glGenTextures(1, &ssaoColorBufferBlur);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// 生成采样核
	// ----------------------
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	//std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// 生成噪点图 随机核心转动
	// ----------------------
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	//unsigned int noiseTexture; 
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	return true;
}
bool OpenglStateSSAO::isUseEBORender()
{
	return false;
}

bool OpenglStateSSAO::isUseFrameBuffer()
{
	return true;
}


int  OpenglStateSSAO::getPointLights()
{
	return 4;
}

bool OpenglStateSSAO::isShowLight()
{
	return false;
}

bool OpenglStateSSAO::isUseCustomLightPos()
{
	return true;
}



string OpenglStateSSAO::getModelFile()
{
	string str("resource/objects/nanosuit/nanosuit.obj");
	return str;
}

bool OpenglStateSSAO::isRenderModel()
{
	return true;
}

bool OpenglStateSSAO::isUseSSAO()
{
	return true;
}

bool OpenglStateSSAO::isUseMSAA()
{
	return false;
}

bool OpenglStateSSAO::isDelayRenderLights()
{
	return false;
}

bool OpenglStateSSAO::isRenderFrameBuffer()
{
	return true;
}


void OpenglStateSSAO::rendeCommand()
{
	
	//__super::rendeCommand();

	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//1. geometry pass: render scene's geometry/color data into gbuffer
	__super::rendeCommand();
	glm::mat4 projection = glm::perspective(glm::radians(_param4), (float)800 / (float)600, 0.1f, 50.0f);
	glm::mat4 model;
	setMat4(_shaderProgram,"projection", &projection);
	setMat4(_shaderProgram,"view", &_viewMat4);

	_projection = projection;

	// room cube
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
	model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
	setMat4(_shaderProgram,"model", &model);
	setInt(_shaderProgram,"invertedNormals", 1); // invert normals as we're inside the cube

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


}

bool OpenglStateSSAO::afterModleRender()
{
	// Send kernel + rotation 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//*********************************************************
	//2 绑定ssao帧缓冲，在ssao的着色器上计算遮蔽因子
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_shaderSSAO);
	for (unsigned int i = 0; i < 64; ++i)
	{
		std::string str = "samples[" + std::to_string(i) + "]";
		setVec3(_shaderSSAO, str.c_str(), ssaoKernel[i]);

	}
	setMat4(_shaderSSAO,"projection", &_projection);

	activiteTexture(GL_TEXTURE0);
	bindTexture(_ssaoPosTexture);
	setInt(_shaderSSAO, "gPosition", 0);

	activiteTexture(GL_TEXTURE1);
	bindTexture(_ssaoNormalTexture);
	setInt(_shaderSSAO, "gNormal", 1);

	activiteTexture(GL_TEXTURE2);
	bindTexture(noiseTexture);
	setInt(_shaderSSAO, "noiseTexture", 2);

	//绘制quad
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//*********************************************************
	//3 绑定模糊帧缓冲，优化环境遮蔽效果，去除噪点
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_shaderSSAOBlur);
	
	activiteTexture(GL_TEXTURE0);
	bindTexture(ssaoColorBuffer);
	setInt(_shaderSSAOBlur, "ssaoInput", 0);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//*********************************************************
	//4. 使用遮蔽因子进行光照计算
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(_shaderLightingPass);
	glm::vec3 lightPosView = glm::vec3(_viewMat4 * glm::vec4(lightPos, 1.0));

	setVec3(_shaderLightingPass,"light.Position", lightPosView);
	setVec3(_shaderLightingPass,"light.Color", lightColor);

	// Update attenuation parameters
	const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const float linear = 0.09;
	const float quadratic = 0.032;
	setFloat(_shaderLightingPass,"light.Linear", linear);
	setFloat(_shaderLightingPass,"light.Quadratic", quadratic);


	activiteTexture(GL_TEXTURE0);
	bindTexture(_ssaoPosTexture);
	setInt(_shaderLightingPass, "gPosition", 0);

	activiteTexture(GL_TEXTURE1);
	bindTexture(_ssaoNormalTexture);
	setInt(_shaderLightingPass, "gNormal", 1);

	activiteTexture(GL_TEXTURE2);
	bindTexture(_ssaoAlbedoSpecTexture);
	setInt(_shaderLightingPass, "gAlbedo", 2);

	activiteTexture(GL_TEXTURE3);
	bindTexture(ssaoColorBufferBlur);
	setInt(_shaderLightingPass, "ssao", 3);


	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	return true;
}

int OpenglStateSSAO::getShaderIndex()
{
	return 42;
}


void OpenglStateSSAO::enableVertexAttribArray()
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
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(cubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


	//恢复VBO VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//绘制一个屏幕大小的矩形 用作帧缓冲最后的输出以及交换数据
	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}



