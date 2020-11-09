#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Materials
{
private:

public:
	glm::vec3 ambient, diffuse, specular;
	float phong;
	Materials(glm::vec3 a, glm::vec3 d, glm::vec3 s, float sh);
	void sendMatToShader(const int shaderID);
};

#endif