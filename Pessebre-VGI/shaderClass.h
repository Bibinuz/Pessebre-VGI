#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID;
	// Constructor predeterminado
	Shader();

	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile);

	void compileErrors(unsigned int shader, const char* type);
	void Activate();
	void Delete(); 
};

#endif