#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "gl3w.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Loader.h"
#include<vector>


class triangle
{
public:
	triangle() {};
	~triangle() {};
	triangle(int type);

	void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection, glm::mat4& offset, int colorNum);
	void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

private:
	void setup();

	/*Triangle Data*/
	int mType;		//Shader ¹øÈ£
	glm::vec3 mKd, mLd;

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;
	ShaderProgram *mShaderProgram;

};
#endif
