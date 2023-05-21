#include "triangle.h"


triangle::triangle(int type)
{
	this->mType = type;
	mKd = glm::vec3(1, 1, 1);
	mLd = glm::vec3(1, 1, 1);
	setup();
}

void triangle::setup()
{

	float Length = 1.0f;

	float cosD = glm::cos(glm::radians(30.0f));
	float sinD = glm::sin(glm::radians(30.0f));

	float tmp = 1 / glm::sqrt(3);

	const float vertexPositions[] = {
		-1 * cosD*Length*tmp, -1 * sinD*Length*tmp, 0.0f, 1.0f,
		0.0f,cosD*Length*tmp,  0.0f, 1.0f,
		cosD*Length*tmp, -1 * sinD*Length*tmp,0.f, 1.0f,
	};

	const float vertexColors[] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1,
		1, 1, 0,
	};

	mShaderProgram = new ShaderProgram();

	//create vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	if (mType == 0)
	{
		//load shaders
		mShaderProgram->initFromFiles("diffuse.vert", "diffuse.frag");

		mShaderProgram->addUniform("LightLocation");
		mShaderProgram->addUniform("Kd");
		mShaderProgram->addUniform("Ld");

		mShaderProgram->addUniform("color");

		mShaderProgram->addUniform("mvp");
		mShaderProgram->addUniform("model");
		mShaderProgram->addUniform("view");
		mShaderProgram->addUniform("projection");
		mShaderProgram->addUniform("location");

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 3, &vertexPositions, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(0);

		//create vbo for colors
		glGenBuffers(1, &vbo_cube_colors);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, &vertexColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(1);

	}

	glBindVertexArray(0);
}

void triangle::Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec4 lightPos(50, 50, 50, 1);

	mShaderProgram->use();

	//simple
	if (mType == 0)
	{
		glUniformMatrix4fv(mShaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		
		glUniform4fv(mShaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
		glUniform3fv(mShaderProgram->uniform("Kd"), 1, glm::value_ptr(mKd));
		glUniform3fv(mShaderProgram->uniform("Ld"), 1, glm::value_ptr(mLd));

	}

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3);

	mShaderProgram->disable();

}

void triangle::Draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection,glm::mat4 & offset,int colorNum)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	glm::vec4 lightPos(50, 50, 50, 1);

	glm::vec3 color(1,1,1);

	switch (colorNum) {
	case 0:
		color = glm::vec3(1, 1, 0);
		break;
	case 1:
		color = glm::vec3(0, 1, 0);
		break;
	case 2:
		color = glm::vec3(0, 0, 1);
		break;
	default:
		color = glm::vec3(1, 1, 1);
		break;
	}
			
	mShaderProgram->use();

	//simple
	if (mType == 0)
	{
		glUniformMatrix4fv(mShaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(mShaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(mShaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(mShaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(mShaderProgram->uniform("location"), 1, GL_FALSE, glm::value_ptr(offset));

		glUniform4fv(mShaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
		glUniform3fv(mShaderProgram->uniform("Kd"), 1, glm::value_ptr(mKd));
		glUniform3fv(mShaderProgram->uniform("Ld"), 1, glm::value_ptr(mLd));

		glUniform3fv(mShaderProgram->uniform("color"), 1, glm::value_ptr(color));

	}

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 4 * 3);

	mShaderProgram->disable();

}
