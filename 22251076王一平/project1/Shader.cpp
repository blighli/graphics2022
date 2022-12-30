#include "Shader.h"

Shader::Shader()
{

}

/*#################################################################
##  函数: Shader
##  函数描述： Shader类的构造函数，用于创建包含Vertex Shader和Fragment
Shader的Shader Program，其ID为programID。
##  参数描述：
##  vertexPath: Vertex Shader的源码路径
##  fragmentPath: Fragment Shader的源码路径
###################################################################*/
Shader::Shader(std::string const vertexPath, std::string const fragmentPath)
{
	// -----------------从文件路径中获取顶点/片段着色器---------------------
	std::string vertexCode;			// Vertex Shader源码字符串
	std::string fragmentCode;		// FragmentShader源码字符串
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		printf("读取Shader文件失败！\n");
		printf("vertexPath:%s\n", vertexPath);
		printf("fragmentPath:%s\n", fragmentPath);
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// -------------------------------编译着色器-------------------------------
	unsigned int vertexShaderID, fragmentShaderID;	// 着色器ID，用于引用着色器
	int success;									// 着色器编译是否成功
	char infoLog[512];
	// 顶点着色器
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);		// 创建顶点着色器
	glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);  // 着色器源码附加到着色器对象上
	glCompileShader(vertexShaderID);						// 编译着色器
	// 若编译出错，则打印错误信息
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		printf("\nVertex Shader编译错误！错误信息：\n%s\n", infoLog);
		printf("Vertex Shader源码：\n%s\n", vShaderCode);
	};

	// 片段着色器
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);	  // 创建Fragment着色器
	glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);  // 着色器源码附加到着色器对象上，第二个参数指定传递源码字符串的数量
	glCompileShader(fragmentShaderID);						  // 编译着色器
	// 若编译出错，则打印错误信息
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success); // 检查是否编译成功
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		printf("\nFragment Shader编译错误！错误信息：\n%s\n", infoLog);
		printf("Fragment Shader源码：\n%s\n", fShaderCode);
	};

	// ------------------------创建并链接着色器程序---------------------------
	programID = glCreateProgram();					// 创建着色器程序
	glAttachShader(programID, vertexShaderID);		// 把着色器对象附加到到着色器程序
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);						// 链接
	// 如果链接失败，打印链接错误
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		printf("链接着色器程序失败！\n错误信息：%s\n", infoLog);
	}

	// 删除着色器对象
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

/*###################################################
##  函数: use
##  函数描述： 使用着色器程序
##  参数描述： 无
#####################################################*/
void Shader::use()
{
	glUseProgram(programID);
}

/*###################################################
##  函数: setBool
##  函数描述： 设置着色器中bool类型的uniform的值
##  参数描述： 
##  name: uniform的名字
##  value: 要设置的uniform的值
#####################################################*/
void Shader::setBool(const std::string &name, bool value) const
{
	// 获得着色器中名为name的uniform的位置值
	int loc = glGetUniformLocation(programID, name.c_str());
	// 更新该uniform的值。注意：更新uniform之前，必须调用glUseProgram
	glUniform1i(loc, (int)value);
}

/*###################################################
##  函数: setInt
##  函数描述： 设置着色器中int类型的uniform的值
##  参数描述：
##  name: uniform的名字
##  value: 要设置的uniform的值
#####################################################*/
void Shader::setInt(const std::string &name, int value) const
{
	// 获得着色器中名为name的uniform的位置值
	int loc = glGetUniformLocation(programID, name.c_str());
	// 更新该uniform的值
	glUniform1i(loc, value);
}

/*###################################################
##  函数: setFloat
##  函数描述： 设置着色器中float类型的uniform的值
##  参数描述：
##  name: uniform的名字
##  value: 要设置的uniform的值
#####################################################*/
void Shader::setFloat(const std::string &name, float value) const
{
	// 获得着色器中名为name的uniform的位置值
	int loc = glGetUniformLocation(programID, name.c_str());
	// 更新该uniform的值
	glUniform1f(loc, value);
}

/*###################################################
##  函数: setVec3
##  函数描述： 设置着色器中vec3类型的uniform的值
##  参数描述：
##  name: uniform的名字
##  value1: 要设置的vec3的x值
##  value2: 要设置的vec3的y值
##  value3: 要设置的vec3的z值
#####################################################*/
void Shader::setVec3(const std::string &name, float value1, float value2, float value3) const
{
	int loc = glGetUniformLocation(programID, name.c_str());
	glUniform3f(loc, value1, value2, value3);
}


void Shader::setMat4(const std::string &name, const glm::mat4 &mat)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}