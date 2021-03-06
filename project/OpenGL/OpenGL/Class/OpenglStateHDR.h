#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateHDR :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
	void rendeCommand();
	int getShaderIndex();
	void enableVertexAttribArray();
	bool isUseFrameBuffer();
	bool isRenderFrameBuffer();
	bool  isUseHDR();
	float getExposure();
};

