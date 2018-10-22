
#include "main.h"
#include <iostream>
#include <map>
#include "OpenglUtils.h"
#include "OpenglCamera.h"
#include "OpenglState.h"
#include "OpenglStateTriangle.h"
#include "OpenglStateRect.h"
#include "OpenglStateMultAttr.h"
#include "OpenglStateTriangleMove.h"
#include "OpenglStateTexture.h"
#include "OpenglStateMultTexture.h"
#include "OpenglStateMultTextureMat4.h"
#include "OpenglStateMultTexture3D.h"
#include "OpenglStateMultTextureCube.h"
#include "OpenglStateMultTextureCamera.h"
#include "OpenglStateMultTextureCameraManuel.h"
#include "OpenglStateMultTextureLight.h"

#include "practice/practice_2_1.h"

float mixValue = 0.2f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

bool firstMouse = true;
float lastX = 400, lastY = 300; //记录鼠标的位置
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

using namespace std;

OpenglCamera *camera = nullptr;


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

	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		//cameraPos += cameraSpeed * cameraFront;
		camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		//cameraPos -= cameraSpeed * cameraFront;
		camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//计算当前帧和上一帧鼠标位置的偏移量
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);

	//float sensitivity = 0.05f;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;

	//yaw += xoffset;
	//pitch += yoffset;


	/*
		对于俯仰角，要让用户不能看向高于89度的地方（在90度时视角会发生逆转，所以我们把89度作为极限），
		同样也不允许小于-89度。这样能够保证用户只能看到天空或脚下，但是不能超越这个限制
	*/
	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;

	////通过俯仰角和偏航角来计算以得到真正的方向向量
	//glm::vec3 front;
	//front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	//front.y = sin(glm::radians(pitch));
	//front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	//cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//if (fov >= 1.0f && fov <= 45.0f)
	//	fov -= yoffset;
	//if (fov <= 1.0f)
	//	fov = 1.0f;
	//if (fov >= 45.0f)
	//	fov = 45.0f;

	camera->ProcessMouseScroll(yoffset);
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

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	OpenglStatesMap[7] = "shader7";
	OpenglStatesMap[8] = "shader8";
	OpenglStatesMap[9] = "shader9";
	OpenglStatesMap[10] = "shader10";
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

	camera = new OpenglCamera();
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


	/*
		GLM库从0.9.9版本起，默认会将矩阵类型初始化为一个零矩阵（所有元素均为0），而不是单位矩阵（对角元素为1，其它元素为0）。
		如果你使用的是0.9.9或0.9.9以上的版本，你需要将所有的矩阵初始化改为 glm::mat4 mat = glm::mat4(1.0f)。
		如果你想与本教程的代码保持一致，请使用低于0.9.9版本的GLM，或者改用上述代码初始化所有的矩阵。
	*/

	intShaders();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	OpenglState *glState = new OpenglStateMultTextureLight();
	int index = glState->getShaderIndex();
	string shaderName = OpenglStatesMap[index];
	string vertFile = "shader/" + shaderName + ".vert";
	string fragFile = "shader/" + shaderName + ".frag";
	glState->init(vertFile, fragFile);
	unsigned int VBO, VAO, EBO;
	VBO = glState->_VBO;
	VAO = glState->_VAO;
	EBO = glState->_EBO;

	unsigned int lightVBO = 0, lighgtVAO = 0, lightEBO = 0;
	if (glState->_isLight)
	{
		lightVBO = glState->_lightVBO;
		lighgtVAO = glState->_lightVAO;
		lightEBO = glState->_lightEBO;
	}



	/*
	glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）。
	glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
	*/
	
	

	// 渲染循环
	glEnable(GL_DEPTH_TEST); // 开启深度测试
	while (!glfwWindowShouldClose(window))
	{
		//记录上一帧所使用的时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 输入
		processInput(window);

		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除颜色缓冲和深度缓冲

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
		glState->_param2 = camera->Position; //更新观察矩阵
		glState->_param3 = camera->Front; //更新观察矩阵
		glState->_param4 = camera->Zoom;   //更新矩阵投影
		glState->rendeCommand();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 解除绑定VAO和VBO：直接绑定为0也是解绑glBindVertexArray(0）
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteBuffers(1, &lightEBO);
	glDeleteVertexArrays(1, &lighgtVAO);
	glDeleteBuffers(1, &lightVBO);

	glfwTerminate(); //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	return 0;
}


