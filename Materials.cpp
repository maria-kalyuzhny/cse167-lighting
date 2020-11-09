#include "Materials.h"

Materials::Materials(glm::vec3 a, glm::vec3 d, glm::vec3 s, float p) {
	ambient = a;
	diffuse = d;
	specular = s;
	phong = p;
}

void Materials::sendMatToShader(const int shaderId) {
	glUseProgram(shaderId);
	glUniform3fv(glGetUniformLocation(shaderId,"ka"),1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shaderId,"kd"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shaderId,"ks"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shaderId,"p"), phong);
	glUseProgram(0);
}