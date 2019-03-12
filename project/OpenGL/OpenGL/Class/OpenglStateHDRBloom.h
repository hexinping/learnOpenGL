#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateHDRBloom :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isUseFrameBuffer();
	bool  isUseHDR();
	float getExposure();
	int getPointLights();
	bool isShowLight();
	bool isUseCustomLightPos();
	bool isUseBloom();
	bool isRenderFrameBuffer();

public:

	//箱子数据
	std::vector<glm::vec3> cubePosition;
	std::vector<glm::vec3> cubeScale;
	std::vector<float> cubeRotate;

	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	unsigned int _shaderBlurProgram;
   
};

