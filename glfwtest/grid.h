#ifndef GRID_H
#define GRID_H

#include "gl3w.h"
#include <glm/mat4x4.hpp>

#include"Loader.h"
#include<vector>

#include<glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<math.h>

class grid {
public:
	grid() { setup(); };

	void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection);

private:
	void setup();

	GLuint vaoHandle;
	GLuint vbo_cube_vertices, vbo_cube_colors;

	/*Grid Data*/
	std::vector<glm::vec4> mVertexPositions;
	ShaderProgram *mShaderProgram;

};


#endif // !LINE_H
