#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "gl3w.h"
#include <glm/mat4x4.hpp>

#include "Loader.h"
#include<vector>


class ellipse
{
public:
	ellipse() {};
	~ellipse() {};
	ellipse(int type);

	void Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection, glm::mat4 &offset, int colorNum);
	void Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection);

private:
	void setup();

	int mType;		//Shader ¹øÈ£
	glm::vec3 mKd, mLd;

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	ShaderProgram *mShaderProgram;


	std::vector<glm::vec4> mVertices;

};
#endif
