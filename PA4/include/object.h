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
    Object(float oRadius, float oSpeed, std::string objPath );
    Object(float oRadius, float oSpeed, std::string objPath, std::string mtlPath);
    void init( float oRadius, float oSpeed );
    int loadObj( std::string fName, std::vector< glm::vec3 > colorList,
                      std::vector< std::string > colorNames );

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
