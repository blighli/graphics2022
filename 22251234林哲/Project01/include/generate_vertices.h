// 顶点生成器：生成几何体的顶点和索引数组

#ifndef __GENERATE_VERTICES_H__
#define __GENERATE_VERTICES_H__

#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include "mesh.h"
#define M_PI 3.14159265358979323846
using namespace std;

// 生成球的顶点和索引
void generate_sphere_vertices(vector<Vertex>& polygon_vertices, vector<unsigned int>& polygon_indices)
{
    polygon_indices.clear();
    polygon_vertices.clear();

    const int xymax = 100;
    const int maximum = xymax * xymax;

    // 顶点数组：根据经纬线在球上取样
    for (int x = 0; x < xymax; x++)
        for (int y = 0; y < xymax; y++)
        {
            float xseg = (float)x / (float)xymax;
            float yseg = (float)y / (float)xymax;
            float xPos = std::cos(xseg * 2.0f * M_PI) * std::sin(yseg * M_PI);
            float yPos = std::cos(yseg * M_PI);
            float zPos = std::sin(xseg * 2.0f * M_PI) * std::sin(yseg * M_PI);
            glm::vec3 pos = glm::vec3(xPos, yPos, zPos);
            polygon_vertices.push_back(Vertex(pos, glm::normalize(pos)));
        }

    // 索引数组：将顶点组成三角面片
    for (int i = 0;i < xymax;i++)
        for (int j = 0;j < xymax;j++)
        {
            polygon_indices.push_back((i * xymax + j) % maximum);
            polygon_indices.push_back(((i + 1) * xymax + j) % maximum);
            polygon_indices.push_back(((i + 1) * xymax + j + 1) % maximum);
            polygon_indices.push_back((i * xymax + j) % maximum);
            polygon_indices.push_back(((i + 1) * xymax + j + 1) % maximum);
            polygon_indices.push_back((i * xymax + j + 1) % maximum);
        }
}

// 生成立方体的顶点和索引
void generate_cube_vertices(vector<Vertex>& polygon_vertices, vector<unsigned int>& polygon_indices)
{
    float cubev[12] = {
        -1.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f,
        1.0f, 0.0f,  1.0f,
        - 1.0f, 0.0f,  1.0f,
    };
    int cubei[6] = {
        0, 1, 2,
        0, 2, 3,
    };
    polygon_vertices.clear();
    polygon_indices.clear();
    for (int i = 0; i < 4; i++)
    {
        glm::vec3 pos(cubev[i * 3], cubev[i * 3 + 1], cubev[i * 3 + 2]);
        polygon_vertices.push_back(Vertex(pos, glm::normalize(pos)));
    }
    for (int i = 0; i < 6; i++)
    {
        polygon_indices.push_back(cubei[i]);
    }
}

#endif