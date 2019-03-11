#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateSSAO :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	bool isUseFrameBuffer();
	int getPointLights();
	bool isShowLight();
	bool isUseCustomLightPos();
	string getModelFile();
	bool isRenderModel();

	bool isUseSSAO();
	bool isUseMSAA();

	bool isDelayRenderLights();
public:

   
};

