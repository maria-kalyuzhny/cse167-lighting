#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "Object.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Model : public Object
{
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::uint> faces; //vertex indices start at 1!
	GLuint VAO;
	GLuint VBOv, VBOn, EBO;

	static glm::uint getVertex(std::string s);
	void loadFromFile(std::string objFilename);
public:
	Model(std::string objFilename);
	~Model();

	void draw(const glm::mat4& view, const glm::mat4& projection, 
		const glm::vec3& eyePos, GLuint shader, bool displayNormals);
	void update();

	void spin(float deg);
	void scale(float ratio);
	void rotate(float angle, glm::vec3 axis);
};

#endif
