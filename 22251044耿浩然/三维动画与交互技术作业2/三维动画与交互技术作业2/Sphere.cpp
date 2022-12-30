#include "Sphere.h"

Sphere::Sphere() 
{
	int m = 90;
	int n = 45;
	vertNum = m * n * 6;
	float Zdegree_step = PI / m;
	float XYdegree_step = (PI * 2) / n;
	float Zdegree = 0;
	float XYdegree = 0;

	for (int i = 0; i < vertNum; i++)
		indices[i] = i;

	int index = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
			float u1, v1, u2, v2, u3, v3, u4, v4;

			//1
			genCoor(x1, y1, z1, Zdegree, XYdegree);
			pushVerts(index, x1, y1, z1);
			genUV(u1, v1, Zdegree, XYdegree);
			pushVerts(index, u1,v1);
			pushVerts(index, x1, y1, z1);
			//2
			genCoor(x2, y2, z2, Zdegree + Zdegree_step, XYdegree);
			pushVerts(index, x2, y2, z2);
			genUV(u2, v2, Zdegree + Zdegree_step, XYdegree);
			pushVerts(index, u2, v2);
			pushVerts(index, x2, y2, z2);
			//3
			genCoor(x3, y3, z3, Zdegree + Zdegree_step, XYdegree + XYdegree_step);
			pushVerts(index, x3, y3, z3);
			genUV(u3, v3, Zdegree + Zdegree_step, XYdegree + XYdegree_step);
			pushVerts(index, u3, v3);
			pushVerts(index, x3, y3, z3);
			//4
			pushVerts(index, x1, y1, z1);
			pushVerts(index, u1, v1);
			pushVerts(index, x1, y1, z1);
			//5
			pushVerts(index, x3, y3, z3);
			pushVerts(index, u3, v3);
			pushVerts(index, x3, y3, z3);
			//6
			genCoor(x4, y4, z4, Zdegree, XYdegree + XYdegree_step);
			pushVerts(index, x4, y4, z4);
			genUV(u4, v4, Zdegree, XYdegree + XYdegree_step);
			pushVerts(index, u4, v4);
			pushVerts(index, x4, y4, z4);

			XYdegree += XYdegree_step;
		}
		XYdegree = 0.0;//每次横向到达2PI角度则横向角度归0
		Zdegree += Zdegree_step;
	}

}//m是纵向细分程度,n是横向细分程度


void Sphere::pushVerts(int &index, float a, float b, float c)
{
	verts[index++] = a;
	verts[index++] = b;
	verts[index++] = c;
}

void Sphere::pushVerts(int &index, float a, float b)
{
	verts[index++] = a;
	verts[index++] = b;
}

void Sphere::genCoor(float& x, float& y, float& z, float Zdegree, float XYdegree)
{
	x = -sin(Zdegree) * cos(XYdegree);
	y = -cos(Zdegree);
	z = sin(Zdegree) * sin(XYdegree);
}

void Sphere::genUV(float& U, float& V, float Zdegree, float XYdegree)
{
	V = Zdegree / PI;
	U = XYdegree / (PI * 2);
}