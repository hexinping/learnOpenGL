#include "OpenglStateShadowDirLight.h"

bool OpenglStateShadowDirLight::init(string vertFile, string fragFile)
{
	float vertices[] = {
		// positions            // normals         // texcoords
		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
		-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

		25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
		-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
		25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 10.0f
	};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);

	_vertFile = vertFile;
	_fragFile = fragFile;

	_texture0 = _glUtils->loadTexture("resource/wood.png");


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


	//把深度信息渲染到一张纹理上
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	//unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0); //为帧缓冲绑定纹理附件
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	postions.push_back(glm::vec3(0.0f, 1.5f, 0.0));
	postions.push_back(glm::vec3(2.0f, 0.0f, 1.0));
	postions.push_back(glm::vec3(-1.0f, 0.0f, 2.0));


	scales.push_back(glm::vec3(0.5));
	scales.push_back(glm::vec3(0.5));
	scales.push_back(glm::vec3(0.25));


	//用于提取深度信息的shader
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/depthMap.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/depthMap.frag");
	_glUtils->linkShader(&_shaderDepthMapProgram, _vertexShader, _fragmentShader);


	//Shader debugDepthQuad("3.1.1.debug_quad.vs", "3.1.1.debug_quad_depth.fs");
	_glUtils->createShaderWithFile(GL_VERTEX_SHADER, &_vertexShader, "shader/depthDebug.vert");
	_glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &_fragmentShader, "shader/depthDebug.frag");
	_glUtils->linkShader(&_shaderDepthDebugProgram, _vertexShader, _fragmentShader);

	return true;
}
bool OpenglStateShadowDirLight::isUseEBORender()
{
	return false;
}

bool OpenglStateShadowDirLight::isLihgtAction()
{
	return true;
}

int  OpenglStateShadowDirLight::getPointLights()
{
	return 1;
}

bool OpenglStateShadowDirLight::isShowLight()
{
	return false;
}

void OpenglStateShadowDirLight::renderScene(unsigned int shader)
{
	glm::mat4 model;
	setMat4(shader, "model", &model);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(cubeVAO);
	for (int i = 0; i < 3; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, postions[i]);
		if (i == 2)
		{
			model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		}
		model = glm::scale(model, scales[i]);
		setMat4(shader, "model", &model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
}

void OpenglStateShadowDirLight::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/
	__super::rendeCommand();
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	setInt(_shaderProgram, "depthDebug", _depthDebug);
	activiteTexture(GL_TEXTURE0);
	bindTexture(_texture0);
	setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0

	if (_depthDebug)
	{
		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		// 使用帧缓冲存储片段的深度信息
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		setMat4(_shaderProgram, "lightSpaceMatrix", &lightSpaceMatrix);
	
		glViewport(0, 0, 800, 600);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	

		glm::mat4 model;
		setMat4(_shaderProgram, "model", &model);

		glDrawArrays(GL_TRIANGLES, 0, 6);


		vector<glm::vec3> postions;
		postions.push_back(glm::vec3(0.0f, 1.5f, 0.0));
		postions.push_back(glm::vec3(2.0f, 0.0f, 1.0));
		postions.push_back(glm::vec3(-1.0f, 0.0f, 2.0));


		vector<glm::vec3> scales;
		scales.push_back(glm::vec3(0.5));
		scales.push_back(glm::vec3(0.5));
		scales.push_back(glm::vec3(0.5));

		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 3;i++)
		{
			model = glm::mat4();
			model = glm::translate(model, postions[i]);
			model = glm::scale(model, scales[i]);
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //把帧缓冲替换为默认的帧缓冲

		// reset viewport
		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------

		glUseProgram(_shaderDepthDebugProgram);
		setFloat(_shaderDepthDebugProgram, "near_plane", near_plane);
		setFloat(_shaderDepthDebugProgram, "far_plane", far_plane);
		activiteTexture(GL_TEXTURE0);
		bindTexture(depthMap);
		setInt(_shaderProgram, "depthMap", 0); // 这里的0就对应了前面的GL_TEXTURE0
	
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

	}
	else if (_useShadow)
	{

		// 不同的shader处理不同的事=======================================

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		// 使用帧缓冲存储片段的深度信息
		
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		

		glUseProgram(_shaderDepthMapProgram);
		setMat4(_shaderDepthMapProgram, "lightSpaceMatrix", &lightSpaceMatrix);

		glViewport(0, 0, 800, 600);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		renderScene(_shaderDepthMapProgram);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0); //把帧缓冲替换为默认的帧缓冲

		// reset viewport
		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// 2. render scene as normal using the generated depth/shadow map  
		// ---------------------------------------------
		__super::rendeCommand();

		setMat4(_shaderProgram, "lightSpaceMatrix", &lightSpaceMatrix);

		activiteTexture(GL_TEXTURE0);
		bindTexture(_texture0);
		setInt(_shaderProgram, "material.diffuse", 0); // 这里的0就对应了前面的GL_TEXTURE0

		activiteTexture(GL_TEXTURE1);
		bindTexture(depthMap);
		setInt(_shaderProgram, "shadowMap", 1); 


		//再render一遍: 真正的绘制到屏幕上，第一次只是为了提取深度信息
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
		setMat4(_shaderProgram, "projection", &projection);

		glm::mat4 view;
		glm::vec3 cameraPos = _param2;
		glm::vec3 cameraFront = _param3;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		setMat4(_shaderProgram, "view", &view);

		setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// 光源的位置
		setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

		setVec3(_shaderProgram, "light.position", lightPos);

		glCullFace(GL_FRONT);
		renderScene(_shaderProgram);
		glCullFace(GL_BACK); // 不要忘记设回原先的culling face


	}
	else
	{
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(_param4), 800.0f / 600.0f, 0.1f, 100.0f);
		setMat4(_shaderProgram, "projection", &projection);

		glm::mat4 view;
		glm::vec3 cameraPos = _param2;
		glm::vec3 cameraFront = _param3;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		setMat4(_shaderProgram, "view", &view);


		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		setVec3(_shaderProgram, "lightPos", lightPos.x, lightPos.y, lightPos.z);		// 光源的位置
		setVec3(_shaderProgram, "viewPos", cameraPos.x, cameraPos.y, cameraPos.z);		// 摄像机的位置（观察空间的原点）

		setVec3(_shaderProgram, "light.position", lightPos);

		glm::mat4 model;
		setMat4(_shaderProgram, "model", &model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		vector<glm::vec3> postions;
		postions.push_back(glm::vec3(0.0f, 1.5f, 0.0));
		postions.push_back(glm::vec3(2.0f, 0.0f, 1.0));
		postions.push_back(glm::vec3(-1.0f, 0.0f, 2.0));


		vector<glm::vec3> scales;
		scales.push_back(glm::vec3(0.5));
		scales.push_back(glm::vec3(0.5));
		scales.push_back(glm::vec3(0.5));

		glBindVertexArray(cubeVAO);

		for (int i = 0; i < 3; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, postions[i]);
			model = glm::scale(model, scales[i]);
			setMat4(_shaderProgram, "model", &model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
	}


	

}

int OpenglStateShadowDirLight::getShaderIndex()
{
	return 38;
}

void OpenglStateShadowDirLight::enableVertexAttribArray()
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


	//画立方体的vao和vbo
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
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(aTexCoordLocation);
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



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

void OpenglStateShadowDirLight::enableLightVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；
}


