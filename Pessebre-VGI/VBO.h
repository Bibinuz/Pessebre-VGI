#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;

	// Constructor completo
	Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex, glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f))
		: position(pos), normal(nor), texUV(tex), color(col) {}

	// Constructor con solo posición
	Vertex(glm::vec3 pos)
		: position(pos), normal(glm::vec3(0.0f, 0.0f, 0.0f)), texUV(glm::vec2(0.0f, 0.0f)), color(glm::vec3(1.0f, 1.0f, 1.0f)) {}
};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};
#endif // !VBO_CLASS_H
