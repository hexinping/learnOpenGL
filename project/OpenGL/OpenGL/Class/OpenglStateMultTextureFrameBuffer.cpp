#include "OpenglStateMultTextureFrameBuffer.h"

bool OpenglStateMultTextureFrameBuffer::init(string vertFile, string fragFile)
{
	float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	int len1 = sizeof(vertices) / sizeof(float);
	memcpy(_vertices, vertices, sizeof(float) * len1);


	_vertFile = vertFile;
	_fragFile = fragFile;
	__super::initRendCommand();


	return true;
}
bool OpenglStateMultTextureFrameBuffer::isUseEBORender()
{
	return false;
}


bool OpenglStateMultTextureFrameBuffer::isRenderFrameBuffer()
{
	return true;
}

void OpenglStateMultTextureFrameBuffer::rendeCommand()
{
	//glUseProgram调用之前设置保持更新
	/*
	但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的。
	*/

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);  // 返回默认,//把创建的帧缓冲对象输出到屏幕，必须要有这句

	//glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	//// clear all relevant buffers
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	//glClear(GL_COLOR_BUFFER_BIT);


	// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _multFrambuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer);
	glBlitFramebuffer(0, 0, 800, 600, 0, 0, 800, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 3. now render quad with scene's visuals as its texture image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);



	__super::rendeCommand();

	activiteTexture(GL_TEXTURE0);
	bindTexture(_textureColorbuffer); //所有的渲染都保存在这个纹理附件对象里_textureColorbuffer，相当于用一张纹理存取了很多绘制信息
	setInt(_shaderProgram, "screenTexture", 0);

	if (_isUseEBORender)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

int OpenglStateMultTextureFrameBuffer::getShaderIndex()
{
	return 22;
}

void OpenglStateMultTextureFrameBuffer::enableVertexAttribArray()
{
	GLint posLocation = _glUtils->getAttribLocation(_shaderProgram, "aPos");
	glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(posLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的


	GLint aTexCoordLocation = _glUtils->getAttribLocation(_shaderProgram, "aTexCoord");
	glVertexAttribPointer(aTexCoordLocation, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(aTexCoordLocation); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的
}

