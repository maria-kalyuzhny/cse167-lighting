#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "P2: Projection & Illumination";

// Objects, materials, lights
Cube * Window::cube;
Model * Window::bunny;
Model * Window::sandal;
Model * Window::bear;
Model * currObj;
Materials * Window::bunnyMat;
Materials * Window::sandalMat;
Materials * Window::bearMat;
Materials * Window::lightMat;
Materials * currMat;
PointLight * Window::light;

// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::shaderProgram; 

// Rotation controls
bool Window::rotate = false;
bool Window::displayNormals = true;
glm::vec3 Window::point1(0,0,0);
glm::vec3 Window::point2(0,0,0);
int Window::mode = 1;

glm::vec3 trackBallMapping(double xpos, double ypos);

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create models for each of the 3 loaded OBJ files
	bunny = new Model("obj/bunny.obj");
	sandal = new Model("obj/SandalF20.obj");
	bear = new Model("obj/bear.obj");

	// Materials for each obj file
	// high diffuse
	bunnyMat = new Materials(glm::vec3(0.4, 0.3, 0.2),
		glm::vec3(0.9,0.6,0.3),
		glm::vec3(0.0,0.0,0.0), 0.1);
	// high specular
	sandalMat = new Materials(glm::vec3(0.2,0.01,0.0),
		glm::vec3(0.0,0.0,0.0), glm::vec3(1.0,0.2,0.1), 100);
	// both
	bearMat = new Materials(glm::vec3(0.0215,0.1745,0.0215),
		glm::vec3(0.07568, 0.61424, 0.07568),
		glm::vec3(0.633, 0.927811, 0.633), 60);

	// Light source
	light = new PointLight(glm::vec3(0,0,8), glm::vec3(1.0,0.8,0.3),
		glm::vec3(0.2,0.2,0.2),"obj/sphere.obj");
	lightMat = new Materials(glm::vec3(1.0,0.9,0.1),glm::vec3(0,0,0),
		glm::vec3(0,0,0), 0);

	// bunny as initial display
	currObj = bunny;
	currMat = bunnyMat;
	bunnyMat->sendMatToShader(shaderProgram);
	light->sendLightToShader(shaderProgram);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete bunny;
	delete sandal;
	delete bear;
	delete bunnyMat;
	delete sandalMat;
	delete bearMat;
	delete light;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	//currObj->update();
	return;
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects
	currMat->sendMatToShader(shaderProgram);
	currObj->draw(view, projection, eyePos, shaderProgram, displayNormals);
	lightMat->sendMatToShader(shaderProgram);
	light->sphere->draw(view, projection, eyePos, shaderProgram, displayNormals);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		// switch between the objects
		case GLFW_KEY_Q:
			currObj = bunny;
			currMat = bunnyMat;
			//bunnyMat->sendMatToShader(shaderProgram);
			break;
		case GLFW_KEY_W:
			currObj = sandal;
			currMat = sandalMat;
			//sandalMat->sendMatToShader(shaderProgram);
			break;
		case GLFW_KEY_E:
			currObj = bear;
			currMat = bearMat;
			//bearMat->sendMatToShader(shaderProgram);
			break;
		//case GLFW_KEY_R:
		//	currObj = light->sphere;
		//	break;
		case GLFW_KEY_1:
			mode = 1;
			break;
		case GLFW_KEY_2:
			mode = 2;
			break;
		case GLFW_KEY_3:
			mode = 3;
			break;
		case GLFW_KEY_N:
			if (displayNormals) {
				displayNormals = false;
			}
			else {
				displayNormals = true; 
			}
			break;

		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	//std::cout<<"scrolling by " << yoffset << std::endl;
	switch (mode){
		case 1:
			if (yoffset < 0) {
				currObj->scale(0.9);
			}
			else if (yoffset > 0) {
				currObj->scale(1.1);
			}
			break;
		case 2:
			//move light source
			if (yoffset < 0) {
				light->moveToCenter(false);
			}
			else if (yoffset > 0) {
				light->moveToCenter(true);
			}
			light->sendLightToShader(shaderProgram);
			break;
		case 3:
			//move light source
			if (yoffset < 0) {
				currObj->scale(0.9);
				light->moveToCenter(true);
			}
			else if (yoffset > 0) {
				currObj->scale(1.1);
				light->moveToCenter(false);
			}
			light->sendLightToShader(shaderProgram);
			break;	
		default:
			break;
	}
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &xpos, &ypos);
		point1 = trackBallMapping(xpos, ypos);
		rotate = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		glfwGetCursorPos(window, &xpos, &ypos);
		rotate = false;
	}
}

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
	if (rotate) {
		//initialize variables and get point2 position
		double xpos, ypos;
		glm::vec3 direction;
		glfwGetCursorPos(window, &xpos, &ypos);
		point2 = trackBallMapping(xpos, ypos);

		//get direction and 'velocity'
		direction = point2-point1;
		float velocity = glm::length(direction);
		//rotate if velocity is sufficiently large to show rotation
		if (velocity > 0.0001) {
			glm::vec3 rotAxis = glm::cross(point1,point2);
			float rotAngle = velocity*2;
			switch (mode) {
			case 1:
				currObj->rotate(rotAngle, rotAxis);
				break;
			case 2:
				light->rotate(rotAngle, rotAxis);
				light->sendLightToShader(shaderProgram);
				break;
			case 3: 
				currObj->rotate(rotAngle, rotAxis);
				light->rotate(rotAngle, rotAxis);
				light->sendLightToShader(shaderProgram);
				break;
			default:
				break;
			}

		}

		//set point1 to current point
		point1 = point2;
	}
}

glm::vec3 trackBallMapping(double xpos, double ypos) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * xpos - Window::width) / Window::width;
	v.y = (Window::height - 2.0 * ypos) / Window::height;
	v.z = 0.0;
	d = glm::length(v);
	d = (d < 1.0) ? d : 1.0;
	v.z = sqrtf(1.001 - d * d);
	v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	//std::cout << "mapping point " << xpos << "," << ypos << " to 3D vector "
	//	<< v.x << "," << v.y << "," << v.z << std::endl;
	return v;
}