#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateModel3DRock :
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

	//glm::mat4 *modelMatrices;
	//unsigned int _amount;
	~OpenglStateModel3DRock();
};

