#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"

class Graphics
{
  public:
    Graphics(std::vector< std::string > graphicFiles);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, float movement[]);
    void Render();

  private:
    std::string ErrorString(GLenum error);
   
    std::vector< std::string > oFiles;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint gSampler;

    Object *rWall;
    Object *lWall;
    Object *tWall;
    Object *cylinder;
    Object *ground;
    Object *ball;
    Object *cube;

    //collision variables
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    

    bool clockwise = true;
};

#endif /* GRAPHICS_H */
