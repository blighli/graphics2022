#pragma once
#include <vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>

#include <tiny_obj_loader.h>

extern unsigned int triangles_id;

class Model
{
public:
    std::vector<GLfloat> vertices;
    std::vector <GLuint> indices;
    int v_size;
    int idx_size;

public:
    Model(std::string path);

};
