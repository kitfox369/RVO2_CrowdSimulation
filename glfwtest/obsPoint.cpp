#include"obstPoint.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

ObstaclePoint::ObstaclePoint(glm::vec2& obstPos)
{
	computeTriPoint(obstPos);

	mShaderProgram = new ShaderProgram();

	mShaderProgram->initFromFiles("red.vert", "red.frag");

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	mShaderProgram->addUniform("mvp");

	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mRectVertices.size()*3, mRectVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ObstaclePoint::Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	mShaderProgram->use();

	glUniformMatrix4fv(mShaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, mRectVertices.size());

	mShaderProgram->disable();

}

void ObstaclePoint::computeTriPoint(glm::vec2 &obstPos)
{
	float temp = 0.5f;
	mRectVertices.push_back(glm::vec3(obstPos.x- temp, obstPos.y + temp, 0));
	mRectVertices.push_back(glm::vec3(obstPos.x+ temp, obstPos.y + temp,  0));
	mRectVertices.push_back(glm::vec3(obstPos.x-temp, obstPos.y - temp, 0));
	mRectVertices.push_back(glm::vec3(obstPos.x-temp, obstPos.y - temp, 0));
	mRectVertices.push_back(glm::vec3(obstPos.x+temp, obstPos.y + temp, 0));
	mRectVertices.push_back(glm::vec3(obstPos.x+temp, obstPos.y - temp, 0));
}

