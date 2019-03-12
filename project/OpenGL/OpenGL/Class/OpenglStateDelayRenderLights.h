#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateDelayRenderLights :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	bool isUseFrameBuffer();
	bool isRenderFrameBuffer();
	int getPointLights();
	bool isShowLight();
	bool isUseCustomLightPos();
	string getModelFile();
	bool isRenderModel();

	bool isDelayRenderLights();
public:

   
};

