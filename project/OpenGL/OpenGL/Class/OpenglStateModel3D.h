#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateModel3D :
	public OpenglState
{
public:
	bool init(string vertFile, string fragFile);
	int getShaderIndex();
	bool isRenderModel();
	void rendeCommand();
	bool isUseReflect();
	bool isUseSkyBox();
	string getModelFile();
	bool isUseFrameBuffer();
	bool isRenderFrameBuffer();
};

