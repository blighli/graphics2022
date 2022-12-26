#include "Shader.h"
#include "RenderUtil.h"

#include <glad/glad.h>
#include <vector>

using namespace std;

Shader::Data::~Data()
{
	glDeleteProgram(id);
};

Shader::Shader()
	: data()
{
	projectionUniform = 0;
	viewUniform = 0;
	modelUniform = 0;
	viewPosUniform = 0;
}

Shader::Shader(std::shared_ptr<Shader::Data> data)
	: data(data)
{
	projectionUniform = this->GetUniformLocation("projection");
	viewUniform = this->GetUniformLocation("view");
	modelUniform = this->GetUniformLocation("model");
	viewPosUniform = this->GetUniformLocation("viewPos");
}

void Shader::Use() const
{
	glUseProgram(data->id);
	glCheckError();
}

GLuint Shader::GetID() const
{
	return data->id;
}

GLint Shader::GetUniformLocation(const std::string &uniformIdentifier) const
{
	return glGetUniformLocation(data->id, uniformIdentifier.c_str());
}

void Shader::SetViewMatrix(const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetProjectionMatrix(const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetModelMatrix(const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, &matrix[0][0]);
	glCheckError();
}

void Shader::SetViewPos(const glm::vec3 &pos) const
{
	glUniform3f(viewPosUniform, pos.x, pos.y, pos.z);
	glCheckError();
}

bool Shader::IsValid()
{
	return data != nullptr;
}

Shader ShaderLoader::BuildFromFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
	return BuildFromFile({{vertexShaderPath, ShaderType::Vertex}, {fragmentShaderPath, ShaderType::Fragment}});
}

Shader ShaderLoader::BuildFromString(const std::string &vertexShader, const std::string &fragmentShader)
{
	return BuildFromString({{vertexShader, ShaderType::Vertex}, {fragmentShader, ShaderType::Fragment}});
}

Shader ShaderLoader::BuildFromFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, const std::string &tcs, const std::string &tes)
{
	return BuildFromFile({{vertexShaderPath, ShaderType::Vertex}, {fragmentShaderPath, ShaderType::Fragment}, {tcs, ShaderType::TessellationContorl}, {tes, ShaderType::TessellationEvaluation}});
}

Shader ShaderLoader::BuildFromFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath, const std::string &geometryShaderPath)
{
	return BuildFromFile({{vertexShaderPath, ShaderType::Vertex}, {fragmentShaderPath, ShaderType::Fragment}, {geometryShaderPath, ShaderType::Geometry}});
}

Shader ShaderLoader::BuildFromFile(const std::unordered_map<std::string, ShaderType> &shaders)
{
	std::unordered_map<std::string, ShaderType> shaderCode;

	for (const auto &keypair : shaders)
	{
		string code;
		ifstream file;

		file.exceptions(ifstream::failbit | ifstream::badbit);
		try
		{
			// 打开文件
			file.open(keypair.first);
			stringstream shaderStream;
			// 读取文件的缓冲内容到数据流中
			shaderStream << file.rdbuf();
			// 关闭文件处理器
			file.close();

			// 转换数据流到string
			code = shaderStream.str();

			shaderCode.emplace(code, keypair.second);
		}
		catch (ifstream::failure e)
		{
			printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n%s\n", keypair.first.c_str());
		}
	}

	return BuildFromString(shaderCode);
}

Shader ShaderLoader::BuildFromString(const std::unordered_map<std::string, ShaderType> &shaders)
{
	GLuint ID = glCreateProgram();
	std::vector<GLuint> shaderID;

	for (const auto &keypair : shaders)
	{
		unsigned int sid = 0;
		switch (keypair.second)
		{
		case ShaderType::Vertex:
			sid = CompileShader(GL_VERTEX_SHADER, keypair.first);
			break;
		case ShaderType::Fragment:
			sid = CompileShader(GL_FRAGMENT_SHADER, keypair.first);
			break;
		case ShaderType::Geometry:
			sid = CompileShader(GL_GEOMETRY_SHADER, keypair.first);
			break;
		case ShaderType::TessellationContorl:
			sid = CompileShader(GL_TESS_CONTROL_SHADER, keypair.first);
			break;
		case ShaderType::TessellationEvaluation:
			sid = CompileShader(GL_TESS_EVALUATION_SHADER, keypair.first);
			break;
		default:
			break;
		}

		shaderID.push_back(sid);
		glAttachShader(ID, sid);
	}

	glLinkProgram(ID);

	// 打印连接错误（如果有的话）
	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
	}

	for (const auto &sid : shaderID)
	{
		glDetachShader(ID, sid);
		glDeleteShader(sid);
	}

	return Shader(std::make_shared<Shader::Data>(ID));
}

unsigned int ShaderLoader::CompileShader(GLenum shaderType, const std::string &shader)
{
	unsigned int shaderID;
	const char *shaderCode = shader.c_str();
	int success;
	char infoLog[512];

	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, NULL);
	glCompileShader(shaderID);
	// 打印编译错误（如果有的话）
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		printf("ERROR::SHADER::COMPILATION_FAILED\n%s", infoLog);
	};

	return shaderID;
}