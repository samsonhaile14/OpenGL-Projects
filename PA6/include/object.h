#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>

#include <Magick++.h>

#include "graphics_headers.h"

class Object
{
  public:
    Object(float oRadius, float oSpeed);
    Object(float oRadius, float oSpeed, std::string objPath);
    void init( float oRadius, float oSpeed );
    int loadObj( std::string fName, std::vector< glm::vec3 > colorList,
                      std::vector< std::string > colorNames );

    ~Object();
    void Update(unsigned int dt, float movement[], bool pause);
    void Render(GLint gSampler);

    void Bind(GLenum TextureUnit, int texIndx);

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<int> indicesStart;
    std::vector<unsigned int> indicesSize;
    GLuint VB;
    GLuint IB;

    std::vector< Magick::Image > skins;
    std::vector< Magick::Blob > blobs;
    std::vector< GLuint > texObjs;

    float orbitAngle;
    float rotAngle;
    float orbitRadius;
    float orbitSpeed;
};

#endif /* OBJECT_H */
