#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "Cube.h"
#include "Model.h"
#include "Materials.h"
#include "PointLight.h"

class Window
{
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to Render
	static Cube* cube;
	static Model* bunny;
	static Model* sandal;
	static Model* bear;
	// Materials
	static Materials* bunnyMat;
	static Materials* sandalMat;
	static Materials* bearMat;
	static Materials* lightMat;
	// Lights
	static PointLight* light;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint shaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	static bool rotate;
	static bool displayNormals;
	static int mode;
	static glm::vec3 point1;
	static glm::vec3 point2;
};

#endif
