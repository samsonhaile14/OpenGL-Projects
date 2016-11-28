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
    Object(float x, float y, float z,float rx, float ry, float rz,float m_mass,int meshType, std::string objPath);
    void init(float m_mass,int meshType);
    int loadObj( std::string fName, std::vector< glm::vec3 > colorList,
                      std::vector< std::string > colorNames );

    ~Object();
    void Update(unsigned int dt, float movement[], bool pause);
    void Render(GLint gSampler, GLint gAmb, GLint gDif, GLint gSpe, GLint shine);

    void Bind(GLenum TextureUnit, int texIndx);

    void setModel(glm::mat4 m);

    glm::mat4 GetModel();
    
    glm::vec3 globalPos;
    glm::vec3 globalRot;
    glm::vec3 globalScale;

    btTriangleMesh *objTriMesh;
    btCollisionShape *shape;
    btDefaultMotionState *shapeMotionState;
    btScalar mass;
    btVector3 inertia;
    btRigidBody *rigidBody;

    glm::mat4 model;

  private:

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<int> indicesStart;
    std::vector<unsigned int> indicesSize;
    GLuint VB;
    GLuint IB;

    std::vector< Magick::Image > skins;
    std::vector< Magick::Blob > blobs;
    std::vector< GLuint > texObjs;

    std::vector<glm::vec4> ambient;
    std::vector<glm::vec4> diffuse;
    std::vector<glm::vec4> specular;
    float shininess;
};

#endif /* OBJECT_H */
