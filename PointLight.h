#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

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
#include "Model.h"

class PointLight
{
private:
	glm::vec3 pos, color, atten;
public:
	Model* sphere;
	PointLight(glm::vec3 p, glm::vec3 c, glm::vec3 a, std::string(objFilename));
	void sendLightToShader(const int shaderId);
	void rotate(float angle, glm::vec3 axis);
	void moveToCenter(bool toCenter);
	//void move()
	~PointLight();
};

#endif

