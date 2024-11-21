#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"
#include <vector>  // Necesario para manejar vectores
#include <string>  // Necesario para manejar rutas y nombres de archivos

class Model
{
public:
    std::vector<Texture> textures; // Vector para almacenar las texturas
    Mesh* objecte;
    int n_objectes;

    Model() { n_objectes = 0; objecte = NULL; }

    // Método para cargar el archivo .obj
    void loadObj(const char* file);

    // Método para cargar el archivo .mtl
    void loadMtl(const char* file);

    // Método para cargar texturas
    GLuint loadTexture(const char* file);

    struct Material {
        glm::vec3 diffuseColor;  // Color difuso
        glm::vec3 specularColor; // Color especular
        float shininess;         // Brillo especular
    };

    Material getMaterial() const { return material; }


private:
    Material material;
};

#endif

