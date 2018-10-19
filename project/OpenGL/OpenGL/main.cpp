
#include "main.h"
#include <iostream>
#include <map>
#include "OpenglUtils.h"
#include "OpenglState.h"
#include "OpenglStateTriangle.h"
#include "OpenglStateRect.h"
#include "OpenglStateMultAttr.h"
#include "OpenglStateTriangleMove.h"
#include "OpenglStateTexture.h"
#include "OpenglStateMultTexture.h"

#include "practice/practice_2_1.h"

float mixValue = 0.2f;

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixValue += 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixValue -= 0.0001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}

}


int createWindow(GLFWwindow** pWindow)
{
	int width = 800;
	int heght = 600;

	GLFWwindow* window = glfwCreateWindow(width, heght, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, heght); //必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)==>设置视口

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	*pWindow = window;
	return 0;
}

map<int, string> OpenglStatesMap;
void intShaders()
{
	OpenglStatesMap[0] = "base";
	OpenglStatesMap[1] = "shader1";
	OpenglStatesMap[2] = "shader2";
	OpenglStatesMap[3] = "shader3";
	OpenglStatesMap[4] = "shader4";
	OpenglStatesMap[5] = "shader5";
	OpenglStatesMap[6] = "shader6";
}

int main(int argc, char* argv[])
{
	glfwInit();

	// 设置opegl版本号为3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//同样明确告诉GLFW我们使用的是核心模式(Core-profile) 明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	GLFWwindow* window;
	createWindow(&window);

	// ------------------------------------------------------------------

	
	//---------------------------------------------------------------------------------------------------
	// OpenglUtils * glUtils = OpenglUtils::getInstance();
	//float vertices[] = {
	//	-0.5f, -0.5f, 0.0f,
	//	0.5f, -0.5f, 0.0f,
	//	0.0f, 0.5f, 0.0f
	//};

	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // 右上角
	//	0.5f, -0.5f, 0.0f,  // 右下角
	//	-0.5f, -0.5f, 0.0f, // 左下角
	//	-0.5f, 0.5f, 0.0f   // 左上角
	//};

	//unsigned int indices[] = { // 注意索引从0开始! 
	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3  // 第二个三角形
	//};

	////1 顶点数据传入
	//unsigned int VBO,VAO,EBO;
	//glUtils->bindVBOAndVAO(&VBO, &VAO, vertices, sizeof(vertices), true, &EBO, indices, sizeof(indices));

	////2 顶点着色器
	//unsigned int vertexShader;
	////createShaderWithSource(GL_VERTEX_SHADER, &vertexShader, vertexShaderSource);
	//glUtils->createShaderWithFile(GL_VERTEX_SHADER, &vertexShader, "shader/base.vert");

	////3片段着色器
	//unsigned int fragmentShader;
	////createShaderWithSource(GL_FRAGMENT_SHADER, &fragmentShader, fragmentShaderSource);
	//glUtils->createShaderWithFile(GL_FRAGMENT_SHADER, &fragmentShader, "shader/base.frag");


	////4链接着色器，创建着色器程序并且激活
	//unsigned int shaderProgram;
	//glUtils->linkShader(&shaderProgram, vertexShader, fragmentShader);

	////5 链接顶点属性
	////第一个参数就是顶点的location值
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	//glEnableVertexAttribArray(0); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的

	//---------------------------------------------------------------------------------------------------

	intShaders();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	OpenglState *glState = new OpenglStateMultTexture();
	int index = glState->getShaderIndex();
	string shaderName = OpenglStatesMap[index];
	string vertFile = "shader/" + shaderName + ".vert";
	string fragFile = "shader/" + shaderName + ".frag";
	glState->init(vertFile, fragFile);
	unsigned int VBO, VAO, EBO;
	VBO = glState->_VBO;
	VAO = glState->_VAO;
	EBO = glState->_EBO;



	/*
	glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
	glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	*/
	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
		//glBindVertexArray(VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针

		//if (isUseEBO)
		//{
		//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //使用索引绘制
		//}
		//else
		//{
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		//}
		glState->_param1 = mixValue;
		glState->rendeCommand();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 解除绑定VAO和VBO：直接绑定为0也是解绑glBindVertexArray(0）
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	return 0;
}


