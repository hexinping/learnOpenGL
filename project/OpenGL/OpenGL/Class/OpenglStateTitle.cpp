#include "OpenglStateTitle.h"

void OpenglStateTitle::rendeCommand()
{
	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RenderText(title, 25.0f, 25.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
	//RenderText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}



