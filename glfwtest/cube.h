#ifndef __CUBE
#define __CUBE

#include "gl3w.h"
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include<vector>


class ColorCube
{
public:
	ColorCube(int type);
	void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, static glm::vec4 RGB);

private:

	void initialize();
	
	int mType;		//Shader ¹øÈ£

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors, vbo_cube_normals, ibo_cube_elements;
	ShaderProgram* mShaderProgram;

	std::vector<glm::vec3> mCubeVertices;
	std::vector<glm::vec3> mCubeNormal;

	void computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

};
#endif