#ifndef _OBSTACLE_POINT_H_
#define _OBSTACLE_POINT_H_

//#include <GL/gl3w.h>
#include "gl3w.h"
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include<vector>
#include"RVOSimulator.h"

class ObstaclePoint {

public:
	ObstaclePoint(glm::vec2 &obstPos);

	void Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection);

private:
	void computeTriPoint(glm::vec2 &obstPos);

	int mType;
	int mVertSize;

	GLuint vaoHandle, vbo_cube_vertices;
	GLuint vbo_line_vertices, vbo_line_colors;
	ShaderProgram *mShaderProgram;

	std::vector<glm::vec3> mRectVertices;
	std::vector<glm::vec3> mOriginVertices;

};

#endif // !_OBSTACLE_H_
