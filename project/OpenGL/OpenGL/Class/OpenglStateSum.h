#pragma once
#include <iostream>
#include "openglstate.h"
class OpenglStateSum :
	public OpenglState
{
public:
	OpenglStateSum();
	bool init(string vertFile, string fragFile);
	bool isUseEBORender();
};

