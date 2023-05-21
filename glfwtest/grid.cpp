#include "grid.h"

void grid::setup()
{
	float gridSize = 60.0f;
	for (int i = -gridSize; i <= gridSize; i += 10)
	{
		mVertexPositions.push_back(glm::vec4(i, -gridSize, 0.0f, 1.0f));
		mVertexPositions.push_back(glm::vec4(i, gridSize, 0.0f, 1.0f));
		mVertexPositions.push_back(glm::vec4(-gridSize, i, 0.0f, 1.0f));
		mVertexPositions.push_back(glm::vec4(gridSize, i, 0.0f, 1.0f));
	}

	mShaderProgram = new ShaderProgram();

	//load shaders
	mShaderProgram->initFromFiles("simple.vert", "simple.frag");

	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	mShaderProgram->addUniform("mvp");

	//create vbo for vertices
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * mVertexPositions.size(), mVertexPositions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

void grid::Draw(glm::mat4 &model, glm::mat4 &view, glm::mat4 & projection)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	mShaderProgram->use();

	glUniformMatrix4fv(mShaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_LINES, 0, 4 * mVertexPositions.size());


}

