#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <string>  // Necesario para std::string
#include "shaderClass.h"

class Texture
{
public:
    GLuint id;               // Identificador de textura en OpenGL (antes era ID)
    std::string type;        // Tipo de textura (e.g., diffuse, specular)
    std::string path;        // Ruta del archivo de la textura
    GLuint unit;             // Unidad de textura

    Texture() { }

    // Constructor que carga la textura
    Texture(const char* img, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

    // Asignar la textura a un shader
    void texUnit(Shader& shader, const char* uniform, GLuint unit);

    // Métodos para manejar la textura
    void Bind();
    void Unbind();
    void Delete();
};

#endif
