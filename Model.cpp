#include "Model.h"

Model::Model(std::string objFilename)
{
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// The color of the model. Try setting it to something else!
	color = glm::vec3(1.0f, 0.8f, 0.1f);

	// parse the file
	loadFromFile(objFilename);

	// find min and max x, y, z values of model
	// TODO replace with aggregate MAX function 
	glm::vec3 min_vec = vertices[0];
	glm::vec3 max_vec = vertices[0];
	for (auto point : vertices) {
		if (point.x < min_vec.x) min_vec.x = point.x;
		if (point.y < min_vec.y) min_vec.y = point.y;
		if (point.z < min_vec.z) min_vec.z = point.z;
		if (point.x > max_vec.x) max_vec.x = point.x;
		if (point.y > max_vec.y) max_vec.y = point.y;
		if (point.z > max_vec.z) max_vec.z = point.z;
	}
	glm::vec3 center = (min_vec + max_vec) * 0.5f;
	//shift all points to center
	//TODO shifting to center and scaling should be done IN THE SHADER
	for (auto& point : vertices) {
		point = point - center;
	}

	//find the max distance from center
	GLfloat max_dist = 0;
	for (auto point : vertices) {
		GLfloat curr_dist = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
		if (curr_dist > max_dist) {
			max_dist = curr_dist;
		}
	}


	//scale by experimentally found value divided by max distance
	//should really be done using model matrix, ew.
	GLfloat scale = 9.6;
	for (auto& point : vertices) {
		point = point * (scale / max_dist);
	}

	// Generate a vertex array (VAO) and vertex buffer objects for vertices and vertex normals.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBOv);
	glGenBuffers(1, &VBOn);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind vertex VBO to the bound VAO, and store the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBOv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass the vertex data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind vertex normals VBO to the VAO, and store normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBOn);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO, and send the index data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uint) * faces.size(), faces.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model()
{
	// Delete the VBO/EBO and the VAO.
	glDeleteBuffers(1, &VBOv);
	glDeleteBuffers(1, &VBOn);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& eyePos,
	GLuint shader, bool displayNormals)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
	glUniform1i(glGetUniformLocation(shader, "displayNormals"), displayNormals);
	glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(eyePos));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Model::update()
{
//	// Spin the cube by 1 degree.
//	spin(0.1f);
	return;
}

//void Model::spin(float deg)
//{
//	// Update the model matrix by multiplying a rotation matrix
//	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
//}

void Model::scale(float ratio)
{
	model = glm::scale(model,glm::vec3(ratio,ratio,ratio));
}

void Model::rotate(float angle, glm::vec3 axis) {
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1), angle, axis);
	model = rotationMatrix*model;
}


void Model::loadFromFile(std::string objFilename) {
	std::ifstream objFile(objFilename);
	if (objFile.is_open())
	{
		std::string line;
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Save the point to points vector
				vertices.push_back(point);
			}

			// If line is normal 
			if (label == "vn")
			{
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				normal = glm::normalize(normal);
				normals.push_back(normal);
			}

			// If line is face
			if (label == "f") {
				std::string v1, v2, v3;
				ss >> v1 >> v2 >> v3;
				faces.push_back(getVertex(v1));
				faces.push_back(getVertex(v2));
				faces.push_back(getVertex(v3));
			}
		}
		std::cout << "Loaded file " << objFilename << std::endl;
	}
	else
	{
		std::cerr << "Error: can't open the file " << objFilename << std::endl;
		return;
	}
}

glm::uint Model::getVertex(std::string s)
{
	std::string::size_type pos = s.find("//");
	if (pos != std::string::npos)
	{
		return std::stoul(s.substr(0, pos))-1;
	}
	else
	{
		//TODO error
		return std::stoul(s);
	}
}
