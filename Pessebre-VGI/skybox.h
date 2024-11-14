
#ifndef SKYBOX_H
#define SKYBOX_H
#include "Model.h"

//Classe Skybox per poder-tractar-la d

class Skybox {
public:
    void initSkybox(const Shader& shader);
    void drawSkybox(Shader& shader, Camera& camera);
    void cleanup();

private:
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO, cubemapTexture;
    const std::vector<std::string> facesCubemap = {
        "./forest-skyboxes/skybox1_px.jpg",
        "./forest-skyboxes/skybox_nx.jpg",
        "./forest-skyboxes/skybox_py.jpg",
        "./forest-skyboxes/skybox_ny.jpg",
        "./forest-skyboxes/skybox_pz.jpg",
        "./forest-skyboxes/skybox_nz.jpg"
    };
};

#endif