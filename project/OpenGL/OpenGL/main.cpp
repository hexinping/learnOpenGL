
#include "main.h"
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>  
#include <fstream>  
#include <streambuf>  
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//const char *vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//
//const char *fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"}\n\0";
//

//.frag .vert文件读取
char* readTheFile(string strSource) {
	//fstream 中判断是否成功打开文件
	//https://blog.csdn.net/fruitz/article/details/41624921
	std::ifstream myfile(strSource);
	//打开失败
	if (myfile.fail()) {
		std::cout << "Can not open it " << std::endl;
		throw new runtime_error("IO stream corrupted");
	}
	//转换成字符串
	std::string str((std::istreambuf_iterator<char>(myfile)),
		std::istreambuf_iterator<char>());
	//关闭文件处理器
	myfile.close();
	//str数组长度一定要 +1,
	/*原因: https://blog.csdn.net/ShiQW5696/article/details/80676290 */
	int len = str.length();
	char* result = new char[len];
	strcpy_s(result, len + 1, str.c_str());
	return result;
}

void bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO)
{
	// 1 顶点数据传入
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	
	glGenVertexArrays(1, VAO); //生成一个vao，只要先画一遍，vao会记录绘制的属性，然后在主循环里不断切换vao就好

	glBindVertexArray(*VAO); //必须先绑定VAO

	glGenBuffers(1, VBO); //生成一个VBO对象:相当于在显存上开辟一块存储空间

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);//glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//把定义的顶点数据复制到缓冲的内存中（VBO）
}

void  createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource)
{
	unsigned int shader  ;
	shader = glCreateShader(shaderType); //创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	glShaderSource(shader, 1, &shaderSource, NULL); //着色器源码附加到着色器对象上
	glCompileShader(shader);//编译

	//判断是否编译成功
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		if (shaderType == GL_VERTEX_SHADER)
		{
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		}
		else
		{
			cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << endl;
		}
		return;

	}
	*pShader = shader;
}

void  createShaderWithFile(int shaderType, unsigned int *pShader, string file)
{
	char * shaderSource = readTheFile(file);
	createShaderWithSource(shaderType, pShader, shaderSource);
}



int linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
	//4链接着色器，创建着色器程序并且激活
	unsigned int program;
	program = glCreateProgram(); //创建着色器程序对象
	glAttachShader(program, vertexShader);  //附加顶点着色器
	glAttachShader(program, fragmentShader);//附加片段着色器
	glLinkProgram(program); // 开始链接顶点着色器和片段着色器

	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::LINK_FAILED\n" << infoLog << endl;
		return -1;
	}

	glDeleteShader(vertexShader); //把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了
	glDeleteShader(fragmentShader);
	*shaderProgram = program;
	return 0;
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

	glViewport(0, 0, width, heght);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// ------------------------------------------------------------------

	//1 顶点数据传入
	unsigned int VBO,VAO;
	bindVBOAndVAO(&VBO, &VAO);

	//2 顶点着色器
	unsigned int vertexShader;
	//createShaderWithSource(GL_VERTEX_SHADER, &vertexShader, vertexShaderSource);
	createShaderWithFile(GL_VERTEX_SHADER, &vertexShader, "shader/base.vert");

	//3片段着色器
	unsigned int fragmentShader;
	//createShaderWithSource(GL_FRAGMENT_SHADER, &fragmentShader, fragmentShaderSource);
	createShaderWithFile(GL_FRAGMENT_SHADER, &fragmentShader, "shader/base.frag");


	//4链接着色器，创建着色器程序并且激活
	unsigned int shaderProgram;
	linkShader(&shaderProgram, vertexShader, fragmentShader);
	

	//5 链接顶点属性
	//第一个参数就是顶点的location值
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上)
	glEnableVertexAttribArray(0); //以顶点属性位置值作为参数，启用顶点属性；顶点属性默认是禁用的


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

		glUseProgram(shaderProgram); //激活着色器程序对象：已激活着色器程序的着色器将在我们发送渲染调用的时候被使用
		glBindVertexArray(VAO);     // 使用VAO后就是每一次渲染的时候直接使用VAO存储好的属性指针
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//6 解除绑定VAO和VBO
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate(); //当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	return 0;
}

