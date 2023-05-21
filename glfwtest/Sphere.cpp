
#ifndef RVO_OUTPUT_TIME_AND_POSITIONS
#define RVO_OUTPUT_TIME_AND_POSITIONS 1
#endif

#include"Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<GLFW\glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>




Sphere::Sphere()
{

}


Sphere::~Sphere()
{
}


//set rad = 1.0  sl, st = 60.
Sphere::Sphere(float rad, GLuint sl, GLuint st,int type) :
	radius(rad), slices(sl), stacks(st)
{
	this->type = type;
	nVerts = (slices + 1) * (stacks + 1);  //the number of vertices
	elements = (slices * 2 * (stacks - 1)) * 3;
	// Vertices
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];
	// Tex coords
	float * tex = new float[2 * nVerts];
	// Elements
	unsigned int * el = new unsigned int[elements];

	// Generate the vertex data : this function fill all data into the arrays.
	generateVerts(v, n, tex, el);

	shaderProgram = new ShaderProgram();

	//create vao, vbo, ibo here...
	if(type==1)			//diffuse Light
	{ 
		shaderProgram->initFromFiles("diffuse.vert", "diffuse.frag");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");

		shaderProgram->addUniform("mvp");
		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("LightLocation");
		shaderProgram->addUniform("Kd");
		shaderProgram->addUniform("Ld");

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 *nVerts, v, GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("coord3d"), // attribute
			3,                 // number of elements per vertex, here (x,y,z,1)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("coord3d"));

		//create vbo for colors
		glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * nVerts,n, GL_STATIC_DRAW);	//VBO에 데이터 저장
		glVertexAttribPointer(
			shaderProgram->attribute("v_normal"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_normal"));

		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*elements, el, GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	else if (type == 2)
	{

		shaderProgram->initFromFiles("Phong.vert", "Phong.frag");

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");
		
		shaderProgram->addUniform("mvp");
		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");
		shaderProgram->addUniform("myMatrix");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");
		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");
	

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *2* 3 * nVerts, v, GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("coord3d"), // attribute
			3,                 // number of elements per vertex, here (x,y,z,1)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("coord3d"));

		//create vbo for colors
		glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *2* 3 * nVerts, n, GL_STATIC_DRAW);	//VBO에 데이터 저장
		glVertexAttribPointer(
			shaderProgram->attribute("v_normal"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_normal"));

		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*elements, el, GL_STATIC_DRAW);

		

		glBindVertexArray(0);


	}

	//texture
	else if (type == 3)
	{
		shaderProgram->initFromFiles("Texture.vert", "Texture.frag");

		int width, height, nrChannels;
		unsigned char *data = stbi_load("earth.jpg", &width, &height, &nrChannels, 0);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shaderProgram->addAttribute("coord3d");
		shaderProgram->addAttribute("v_normal");
		shaderProgram->addAttribute("aTexCoord");
		shaderProgram->addAttribute("instance");

		shaderProgram->addUniform("mvp");
		shaderProgram->addUniform("ModelViewMatrix");
		shaderProgram->addUniform("NormalMatrix");

		shaderProgram->addUniform("Light.Position");
		shaderProgram->addUniform("Light.La");
		shaderProgram->addUniform("Light.Ld");
		shaderProgram->addUniform("Light.Ls");

		shaderProgram->addUniform("Material.Ka");
		shaderProgram->addUniform("Material.Kd");
		shaderProgram->addUniform("Material.Ks");
		shaderProgram->addUniform("Material.Shiness");

		//create vbo for vertices
		glGenBuffers(1, &vbo_cube_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nVerts, v, GL_STATIC_DRAW);
		glVertexAttribPointer(
			shaderProgram->attribute("coord3d"), // attribute
			3,                 // number of elements per vertex, here (x,y,z,1)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("coord3d"));

		//create vbo for colors
		glGenBuffers(1, &vbo_cube_normals);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * nVerts, n, GL_STATIC_DRAW);	//VBO에 데이터 저장
		glVertexAttribPointer(
			shaderProgram->attribute("v_normal"), // attribute
			3,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("v_normal"));

		glGenBuffers(1, &vbo_cube_texture);						//Color VBO 생성
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texture);			//버퍼를 activate("지금 이것을 다룬다")
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * nVerts, tex, GL_STATIC_DRAW);	//VBO에 데이터 저장
		glVertexAttribPointer(
			shaderProgram->attribute("aTexCoord"), // attribute
			2,                 // number of elements per vertex, here (R,G,B)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
		);
		glEnableVertexAttribArray(shaderProgram->attribute("aTexCoord"));

		glGenBuffers(1, &ibo_cube_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*elements, el, GL_STATIC_DRAW);

		glBindVertexArray(0);


	}

}


void Sphere::draw(glm::mat4 & model, glm::mat4 & view, glm::mat4 & projection, static glm::vec4 RGB, static glm::vec3 LightPos)
{

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));


	//glm::vec4 lightPos(50, 50, 50, 1);
	glm::vec4 lightPos(LightPos, 1.0);
	glm::vec3 kd(1, 1, 1);
	glm::vec3 ld(1, 1, 1);

	shaderProgram->use();

	if (type == 1)
	{

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform4fv(shaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Kd"), 1, glm::value_ptr(kd));
		glUniform3fv(shaderProgram->uniform("Ld"), 1, glm::value_ptr(ld));

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);

		shaderProgram->disable();

	}

	else if (type == 2)
	{

		glm::mat4 myMatrix = glm::translate(glm::mat4(1.f), glm::vec3(1.f, 1.f, 1.f));

		glm::mat4 scaleMatrix = glm::scale(glm::vec3(1, 1, 1));


		glm::vec3 skelMatrix[10] = {
			{glm::vec3(0,0,0)},
			{ glm::vec3(0,-2,0) },
			{ glm::vec3(2,-2,-2) },
			{ glm::vec3(-2,-2,2) },
			{ glm::vec3(0,-4,0) },
			{ glm::vec3(1,-6,-1) },
			{ glm::vec3(-1,-6,1) },
			{ glm::vec3(1,-8,-1) },
			{ glm::vec3(-1,-8,1) },
			{ glm::vec3(0,-4,0) },
		};

		

		glm::vec3 Ka(0.1, 0.1, 0.1);
		glm::vec3 Kd(RGB.x,RGB.y,RGB.z);
		//glm::vec3 Kd(1, 1, 0);
		glm::vec3 Ks(1, 1, 0);
		GLfloat shiness = 10;

		glm::vec3 La(0.9, 0.9, 0.9);
		glm::vec3 Ld(0.6, 0.6, 0.6);
		glm::vec3 Ls(0.6, 0.6, 0.6);

		//glm::vec3 index(1, 0, 0);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
		glUniformMatrix4fv(shaderProgram->uniform("myMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));
		



		

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);



		glBindVertexArray(VAO);

		glm::vec3 index(0, 0, 0);
		glUniform3fv(shaderProgram->uniform("index"), 1, glm::value_ptr(index));
		//glDrawElementsInstanced(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0,10);

		
		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);

		shaderProgram->disable();

	}

	else if (type == 3)
	{

		glm::vec3 Ka(0.1, 0.1, 0.1);
		glm::vec3 Kd(RGB.x, RGB.y, RGB.z);
		//glm::vec3 Kd(1, 1, 0);
		glm::vec3 Ks(1, 1, 1);
		GLfloat shiness = 10;

		glm::vec3 La(1, 1,1);
		glm::vec3 Ld(1, 1, 1);
		glm::vec3 Ls(1, 1, 1);

		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
		glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

		glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
		glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
		glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
		glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

		glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
		glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
		glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
		glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT, 0);

		shaderProgram->disable();

	}

	//glDrawArraysInstanced(GL_TRIANGLES, 0, nVerts * 3, 10);
}



void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;



	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;

		for (GLuint i = 0; i < slices; i++) {
			GLuint stackStart = i * (stacks + 1);
			GLuint nextStackStart = (i + 1) * (stacks + 1);
			for (GLuint j = 0; j < stacks; j++) {
				if (j == 0) {
					el[idx] = stackStart;
					el[idx + 1] = stackStart + 1;
					el[idx + 2] = nextStackStart + 1;
					idx += 3;
				}
				else if (j == stacks - 1) {
					el[idx] = stackStart + j;
					el[idx + 1] = stackStart + j + 1;
					el[idx + 2] = nextStackStart + j;
					idx += 3;
				}
				else {
					el[idx] = stackStart + j;
					el[idx + 1] = stackStart + j + 1;
					el[idx + 2] = nextStackStart + j + 1;
					el[idx + 3] = nextStackStart + j;
					el[idx + 4] = stackStart + j;
					el[idx + 5] = nextStackStart + j + 1;
					idx += 6;
				}
			}

	}
}

int Sphere::getVertexArrayHandle()
{
	return this->VAO;
}

