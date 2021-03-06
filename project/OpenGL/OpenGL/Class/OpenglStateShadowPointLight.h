#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateShadowPointLight :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	void enableLightVertexAttribArray();
	bool isLihgtAction();
	int getPointLights();
	bool isShowLight();
	void renderScene(unsigned int shader);
public:
	unsigned int depthMapFBO;   //储存深度信息的帧缓冲
	unsigned int depthCubemap;      //深度贴图
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	//绘制到屏幕上的vao vbo
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	unsigned int _shaderDepthDebugProgram;
	unsigned int _shaderDepthMapProgram;  //提取深度信息用

	bool  _depthDebug;
	bool  _usePCF;

	vector<glm::vec3> postions;
	vector<glm::vec3> scales;

	unsigned int _geometryShader;
	glm::vec3 lightPos;
};

