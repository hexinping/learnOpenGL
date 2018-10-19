#include "OpenglUtils.h"
#include "std_image.h"

static OpenglUtils *glUtils = nullptr;

OpenglUtils * OpenglUtils::getInstance()
{
	if (!glUtils)
	{
		glUtils = new (std::nothrow) OpenglUtils();
	}
	return glUtils;
}

char* OpenglUtils::readTheFile(string strSource) {
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

void OpenglUtils::bindVBOAndVAO(unsigned int *VBO, unsigned int *VAO, float vertices[], int verticesLenght, bool useEBO, unsigned int *EBO, unsigned int indices[], int indicesLenght)
{
	// 1 顶点数据传入
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

	glGenVertexArrays(1, VAO); //生成一个vao，只要先画一遍，vao会记录绘制的属性，然后在主循环里不断切换vao就好

	glBindVertexArray(*VAO); //必须先绑定VAO

	glGenBuffers(1, VBO); //生成一个VBO对象:相当于在显存上开辟一块存储空间

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);//glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上

	glBufferData(GL_ARRAY_BUFFER, verticesLenght, vertices, GL_STATIC_DRAW);//把定义的顶点数据复制到缓冲的内存中（VBO）


	//复制我们的索引数组到一个索引缓冲中，供OpenGL使用
	if (useEBO)
	{
		glGenBuffers(1, EBO); //创建一个缓冲区对象
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO); //glBindBuffer函数把新创建的缓冲绑定到GL_ELEMENT_ARRAY_BUFFER目标上
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLenght, indices, GL_STATIC_DRAW); //把定义的顶点数据复制到缓冲的内存中（EBO）
	}

}

void  OpenglUtils::createShaderWithSource(int shaderType, unsigned int *pShader, const char *shaderSource)
{
	unsigned int shader;
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

void  OpenglUtils::createShaderWithFile(int shaderType, unsigned int *pShader, string file)
{
	char * shaderSource = readTheFile(file);
	createShaderWithSource(shaderType, pShader, shaderSource);
}

int OpenglUtils::linkShader(unsigned int *shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
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

GLint OpenglUtils::getUniformLocation(unsigned int program, const char *name)
{
	return glGetUniformLocation(program, name);
}

GLint OpenglUtils::getAttribLocation(unsigned int program, const char *name)
{
	return glGetAttribLocation(program, name);
}

void OpenglUtils::setBool(unsigned int program, const char *name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name), (int)value);
}
void OpenglUtils::setInt(unsigned int program, const char *name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name), value);
}
void OpenglUtils::setFloat(unsigned int program, const char *name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name), value);
}

void OpenglUtils::genTexImage2D(const char *file, int type, int level, int textureUnit, int around, int filter)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(textureUnit); // 在绑定纹理之前先激活纹理单元

	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, around);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, around);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);


	//加载纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, level, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_set_flip_vertically_on_load(true);
	stbi_image_free(data);

}

void OpenglUtils::useProgram(unsigned int program)
{
	glUseProgram(program);
}
