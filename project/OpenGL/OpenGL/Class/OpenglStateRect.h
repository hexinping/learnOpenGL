#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateRect :
	public OpenglState
{
public:
	OpenglStateRect();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
};

