#include"ObstacleRect.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

ObstacleRect::ObstacleRect(std::vector<RVO::Vector2> &obstPos)
{
	mVertSize = obstPos.size();

	for(int i=0;i<obstPos.size();i++)
		mRectVertices.push_back(glm::vec3(obstPos[i].x(), obstPos[i].y(),0));

	mShaderProgram = new ShaderProgram();

	mShaderProgram->initFromFiles("black.vert", "black.frag");

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	mShaderProgram->addUniform("mvp");
	
	glGenBuffers(1, &vbo_cube_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mRectVertices.size()* 3, mRectVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void ObstacleRect:: computeTriPoint(std::vector<RVO::Vector2> obstPos)
{
	mRectVertices.push_back(glm::vec3(obstPos[0].x(), obstPos[0].y(),0));
	mRectVertices.push_back(glm::vec3(obstPos[1].x(), obstPos[1].y(), 0));
	mRectVertices.push_back(glm::vec3(obstPos[2].x(), obstPos[2].y(), 0));
	mRectVertices.push_back(glm::vec3(obstPos[0].x(), obstPos[0].y(), 0));
	mRectVertices.push_back(glm::vec3(obstPos[2].x(), obstPos[2].y(), 0));
	mRectVertices.push_back(glm::vec3(obstPos[3].x(), obstPos[3].y(), 0));
}

void ObstacleRect::Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	mShaderProgram->use();

		glUniformMatrix4fv(mShaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_TRIANGLE_FAN, 0, mRectVertices.size());

	mShaderProgram->disable();

}
