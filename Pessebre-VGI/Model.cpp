#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb/stb_image.h"  // Asegúrate de incluir stb_image.h si estás usando esta librería para cargar texturas

void Model::loadObj(const char* file)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    textures.clear();  // Limpiar texturas cargadas previamente, si las hay.

    std::vector<glm::vec3> tempVert;
    std::vector<glm::vec3> tempNor;
    std::vector<glm::vec2> tempTex;
    std::vector<int> tempIndex;
    std::vector<int> tempTexIndex;
    std::vector<int> tempNorIndex;

    std::ifstream in(file, std::ios::in);
    if (!in) {
        std::cerr << "Cannot open " << file << std::endl;
        exit(1);
    }

    std::string line;
    std::string materialFile;  // Para almacenar el archivo .mtl

    while (std::getline(in, line)) {

        // Buscar el archivo .mtl
        if (line.substr(0, 6) == "mtllib") {
            std::istringstream v(line.substr(7));
            v >> materialFile;
            std::string objPath = file;  // Obtén la ruta del archivo .obj
            std::string directory = objPath.substr(0, objPath.find_last_of('/') + 1);  // Extrae la ruta
            std::string fullPath = directory + materialFile;  // Combina la ruta con el archivo .mtl
            loadMtl(fullPath.c_str());  // Cargar el archivo .mtl asociado
        }

        if (line.substr(0, 2) == "v ") {
            std::istringstream v(line.substr(2));
            glm::vec3 vert;
            double x, y, z;
            v >> x; v >> y; v >> z;
            vert = glm::vec3(x, y, z);
            tempVert.push_back(vert);
        }
        else if (line.substr(0, 2) == "vn") {
            std::istringstream v(line.substr(3));
            glm::vec3 nor;
            double x, y, z;
            v >> x; v >> y; v >> z;
            nor = glm::vec3(x, y, z);
            tempNor.push_back(nor);
        }
        else if (line.substr(0, 2) == "vt") {
            std::istringstream v(line.substr(3));
            glm::vec2 tex;
            float U, V;
            v >> U; v >> V;
            tex = glm::vec2(U, V);
            tempTex.push_back(tex);
        }
        else if (line.substr(0, 2) == "f ") {
            std::istringstream v(line.substr(2));
            int vertexIndex[3], texIndex[3], normalIndex[3];
            char slash;

            // Leer tres índices para formar un triángulo (puedes adaptarlo si hay más de tres)
            for (int i = 0; i < 3; i++) {
                v >> vertexIndex[i] >> slash >> texIndex[i] >> slash >> normalIndex[i];
                // Convertir de índice 1-based a índice 0-based
                vertexIndex[i]--;
                texIndex[i]--;
                normalIndex[i]--;

                // Crear y agregar el vértice al vector de vértices
                Vertex vert = {
                    tempVert[vertexIndex[i]],
                    tempNor[normalIndex[i]],
                    tempTex[texIndex[i]]
                };
                vertices.push_back(vert);
                indices.push_back(vertices.size() - 1);
            }
        }
    }

    for (unsigned int i = 0; i < tempIndex.size(); i++) {
        Vertex vert = { tempVert[tempIndex[i]], tempNor[tempNorIndex[i]], tempTex[tempTexIndex[i]] };
        vertices.push_back(vert);
        indices.push_back(i);
    }

    objecte = new Mesh(vertices, indices, textures);
}

void Model::loadMtl(const char* file)
{
    std::ifstream in(file, std::ios::in);
    if (!in) {
        std::cerr << "Cannot open " << file << std::endl;
        exit(1);
    }

    std::string line;
    std::string currentMaterial;
    std::string texturePath;

    // Obtener el directorio del archivo .mtl para formar la ruta completa a las texturas
    std::string mtlDirectory = std::string(file);
    mtlDirectory = mtlDirectory.substr(0, mtlDirectory.find_last_of('/') + 1);

    Material material;  // Aquí defines la variable 'material'

    while (std::getline(in, line)) {
        if (line.substr(0, 7) == "newmtl ") {
            currentMaterial = line.substr(7);
        }
        else if (line.substr(0, 7) == "map_Kd ") {
            texturePath = line.substr(7);

            // Combinar el directorio del .mtl con el path de la textura para obtener la ruta completa
            std::string fullTexturePath = mtlDirectory + texturePath;

            std::cout << "Cargando textura mtl desde: " << fullTexturePath << std::endl;

            Texture texture;
            texture.id = loadTexture(fullTexturePath.c_str());
            texture.type = "diffuse";
            texture.path = fullTexturePath;
            textures.push_back(texture);
        }
        else if (line.substr(0, 3) == "Kd ") {  // Color difuso
            std::istringstream colorStream(line.substr(3));
            colorStream >> material.diffuseColor.r >> material.diffuseColor.g >> material.diffuseColor.b;
            std::cout << "Usando color difuso: ("
                << material.diffuseColor.r << ", "
                << material.diffuseColor.g << ", "
                << material.diffuseColor.b << ")" << std::endl; // Mensaje de depuración
        }
        else if (line.substr(0, 3) == "Ks ") {  // Color especular
            std::istringstream colorStream(line.substr(3));
            colorStream >> material.specularColor.r >> material.specularColor.g >> material.specularColor.b;
            std::cout << "Usando color especular: ("
                << material.specularColor.r << ", "
                << material.specularColor.g << ", "
                << material.specularColor.b << ")" << std::endl; // Mensaje de depuración
        }
        else if (line.substr(0, 3) == "Ns ") {  // Brillo especular
            material.shininess = std::stof(line.substr(3));
            std::cout << "Usando brillo especular: " << material.shininess << std::endl; // Mensaje de depuración
        }
    }

    // Si no se ha cargado una textura, mostrar mensaje de error
    if (textures.empty()) {
        std::cout << "No se encontró mapa de textura, usando color difuso en lugar de textura." << std::endl;

        GLuint solidTextureID;
        glGenTextures(1, &solidTextureID);
        glBindTexture(GL_TEXTURE_2D, solidTextureID);
        glGenerateMipmap(GL_TEXTURE_2D);

        Texture texture;
        texture.id = solidTextureID;
        texture.type = "diffuse";
        textures.push_back(texture);
    }
}


GLuint Model::loadTexture(const char* file) {
    std::cout << "Cargando textura desde: " << file << std::endl;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum internalFormat, format;

        // Determinar los formatos correctos según el número de canales de la imagen
        if (nrChannels == 1) {
            internalFormat = GL_R8;  // Para una textura en escala de grises
            format = GL_RED;
        }
        else if (nrChannels == 3) {
            internalFormat = GL_SRGB;   // Textura RGB en espacio de color sRGB
            format = GL_RGB;
        }
        else if (nrChannels == 4) {
            internalFormat = GL_SRGB_ALPHA;   // Textura RGBA en espacio de color sRGB
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);

        // Cargar la textura con el formato adecuado
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Configurar parámetros de la textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        std::cout << "Textura cargada correctamente: " << file << std::endl;
    }
    else {
        std::cerr << "Failed to load texture: " << file << std::endl;
        std::cerr << "Reason: " << stbi_failure_reason() << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


