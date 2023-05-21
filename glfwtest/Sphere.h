#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "gl3w.h"
//#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include<vector>
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION

#include<sstream>


//
//#include <cmath>
//#include <cstddef>
//#include <vector>
//
//#if RVO_OUTPUT_TIME_AND_POSITIONS
//#include <iostream>
//#endif


class Sphere {

private:
	float radius;
	GLuint slices;
	GLuint stacks;

	float nVerts,elements;

	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements, vbo_cube_texture, instanceVBO;

	GLuint VAO;

	ShaderProgram *shaderProgram;

	int type;			//Shader Á¾·ù

public:

	int R;
	int G;
	int B;

	int index;

	Sphere();
	~Sphere();
	Sphere(float rad, GLuint sl, GLuint st,int type);
	void Sphere::draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection,glm::vec4 RGB, static glm::vec3 LightPos);
	void Sphere::generateVerts(float * verts, float * norms, float * tex, unsigned int * el);
	
	int Sphere::getVertexArrayHandle();

};



#endif // !_SPHERE_H_
