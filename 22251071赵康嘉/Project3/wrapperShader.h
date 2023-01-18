
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class wrapperShader
{
public:
	wrapperShader();
	~wrapperShader();

	static wrapperShader createWithFilePath(const char *vPath, const char *fPath);
	static wrapperShader create(const char *vSource, const char *fSource);

	void useShaderProgram();
	GLuint getId(){
		return m_shaderProgram;
	}

	void setVec3(const char *name, float, float,float);
	void setVec3(const char *name, glm::vec3 parm);
	void setMat4fv(const char *name, glm::mat4 parm);
	void set1i(const char *name, int parm);
	void set1f(const char *name, float parm);
private:
	bool init(const char *vSource, const char *fSource);

	GLuint m_shaderProgram;
};
