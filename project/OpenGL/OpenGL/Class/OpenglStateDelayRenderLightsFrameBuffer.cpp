#include "OpenglStateDelayRenderLightsFrameBuffer.h"

bool OpenglStateDelayRenderLightsFrameBuffer::init(string vertFile, string fragFile)
{
	float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);


	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();

	//设置纹理单元
	_glUtils->useProgram(_shaderProgram);// 先使用这个着色器程序对象才能设置uniform变量

	//点光源
	_lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	_lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	_lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	_lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));


	_lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	_lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	_lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	_lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));


	//_lightColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	//_lightColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	//_lightColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	//_lightColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));


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


	setInt(_shaderProgram,"gPosition", 0);
	setInt(_shaderProgram,"gNormal", 1);
	setInt(_shaderProgram,"gAlbedoSpec", 2);

	return true;
}
bool OpenglStateDelayRenderLightsFrameBuffer::isUseEBORender()
{
	return false;
}


bool OpenglStateDelayRenderLightsFrameBuffer::isRenderFrameBuffer()
{
	return true;
}

void OpenglStateDelayRenderLightsFrameBuffer::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/

	//非延迟光照才使用多重缓冲帧缓冲区
	if (!_isUseDelayeRenderLight)
	{
		//采用多重采样帧缓冲
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _multFrambuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
		glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);// 把多重采样的帧缓冲数据复制到正常帧缓冲内容里
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  // 返回默认,//把创建的帧缓冲对象输出到屏幕，必须要有这句
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);


	__super::rendeCommand();


	//activiteTexture(GL_TEXTURE0);
	//bindTexture(_textureColorbuffer); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	//setInt(_shaderProgram, "screenTexture", 0);

	//activiteTexture(GL_TEXTURE1);
	//bindTexture(_blurTexture); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	//setInt(_shaderProgram, "bloomBlur", 1);


	activiteTexture(GL_TEXTURE0);
	bindTexture(_delayeRenderLightPosTexture); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	setInt(_shaderProgram, "gPosition", 0);

	activiteTexture(GL_TEXTURE1);
	bindTexture(_delayeRenderLightNormalTexture); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	setInt(_shaderProgram, "gNormal", 1);


	activiteTexture(GL_TEXTURE2);
	bindTexture(_delayeRenderLightAlbedoSpecTexture); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	setInt(_shaderProgram, "gAlbedoSpec", 2);

	setInt(_shaderProgram, "useHDR", true);
	setInt(_shaderProgram, "useDelayRenderLight", true);
	glm::vec3 cameraPos = _param2;
	setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);


	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}



}

int OpenglStateDelayRenderLightsFrameBuffer::getShaderIndex()
{
	return 37;
}

void OpenglStateDelayRenderLightsFrameBuffer::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

