#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>
#include "graphics_headers.h"

class Object
{
  public:
    Object(float oRadius, float oSpeed);
    Object(float oRadius, float oSpeed, std::string objPath);
    void init( float oRadius, float oSpeed );
    ~Object();
    void Update(unsigned int dt, float movement[], bool pause);
    void Render();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float orbitAngle;
    float rotAngle;
    float orbitRadius;
    float orbitSpeed;
};

#endif /* OBJECT_H */
