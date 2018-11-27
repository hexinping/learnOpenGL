
#include "main.h"
#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include "OpenglUtils.h"
#include "OpenglCamera.h"


#define random(a,b) (rand()%(b-a+1)+a)

#define MAX_SHADERCOUNT 39


int width = 800;
int height = 600;

float mixValue = 0.2f;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

bool firstMouse = true;
float lastX = 400, lastY = 300; //记录鼠标的位置
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
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

	GLFWwindow* window = glfwCreateWindow(width, height, "BreakOut", NULL, NULL);
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

	glViewport(0, 0, width, height); //必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)==>设置视口

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	*pWindow = window;
	return 0;
}

map<int, string> OpenglStatesMap;
void initShaders()
{
	int count = MAX_SHADERCOUNT;
	for (int i = 0; i <= count;i++)
	{
		string shaderName = "shader" + to_string(i);
		OpenglStatesMap.insert(pair<int, string >(i, shaderName));
	}
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

	//glfwWindowHint(GLFW_SAMPLES, 4); //每个像素点使用4个采样点 ，为了多级采样用
	GLFWwindow* window;
	createWindow(&window);

	camera = new OpenglCamera();

	while (!glfwWindowShouldClose(window))
	{
		//记录上一帧所使用的时间
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 输入
		processInput(window);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete camera;

	glfwTerminate(); //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	return 0;
}


