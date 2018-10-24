#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateModel3D :
	public OpenglState
{
public:
	OpenglStateModel3D();
	bool init(string vertFile, string fragFile);
	int getShaderIndex();
};

