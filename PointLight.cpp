#include "PointLight.h"

PointLight::PointLight(glm::vec3 p, glm::vec3 c, glm::vec3 a,
	std::string objFilename) {
	pos = p;
	color = c;
	atten = a;
	sphere = new Model(objFilename);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1),glm::vec3(0.05));
	sphere->model = scaleMatrix * sphere->model;
	glm::mat4 moveMatrix = glm::translate(glm::mat4(1),pos);
	sphere->model = moveMatrix * sphere->model;
}

void PointLight::sendLightToShader(const int shaderId) {
	glUseProgram(shaderId);
	glUniform3fv(glGetUniformLocation(shaderId, "lightPos"), 1, glm::value_ptr(pos));
	glUniform3fv(glGetUniformLocation(shaderId, "lightCol"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderId, "lightAtten"), 1, glm::value_ptr(atten));
	glUseProgram(0);
}

void PointLight::rotate(float angle, glm::vec3 axis) {
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), angle, axis);
	sphere->model = rotationMatrix * sphere->model;
	pos = glm::vec3(rotationMatrix * glm::vec4(pos,1));
}

void PointLight::moveToCenter(bool toCenter) {

	glm::mat4 translationMatrix = glm::mat4(1);
	if (toCenter && glm::length(pos)>=0.5) {
		translationMatrix = glm::translate(glm::mat4(1), 0.1f*(-pos));
	}
	else if (!toCenter && glm::length(pos)<= 12) {
		translationMatrix = glm::translate(glm::mat4(1), 0.1f*(pos));
	}
	sphere->model = translationMatrix * sphere->model;
	pos = glm::vec3(translationMatrix * glm::vec4(pos, 1));
}

PointLight::~PointLight() {
	delete sphere;
}
