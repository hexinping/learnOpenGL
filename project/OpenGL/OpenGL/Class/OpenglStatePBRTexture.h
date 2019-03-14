#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStatePBRTexture :
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
	bool isRenderFrameBuffer();

	bool isDelayRenderLights();
	void enableVertexAttribArray();



public:
	unsigned int sphereVAO = 0;
	unsigned int indexCount = 0;

};



