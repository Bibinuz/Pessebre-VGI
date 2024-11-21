#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

enum TipusLlum
{
	Punt,
	Foco,
	Directa
};

struct Llum {
	bool sw_light;
	glm::vec3 lightPos;
	glm::vec4 lightCol;
	TipusLlum tipus;
	float intensitat;
};

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	// Constructor predeterminado
	Shader();

	Shader(const char* vertexFile, const char* fragmentFile);

	void compileErrors(unsigned int shader, const char* type);
	void Activate();
	void Delete(); 
};

#endif