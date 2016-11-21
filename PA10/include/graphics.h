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
    Graphics(float dimness, float ambDimness);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, float movement[]);
    void Render();
    bool loadShaderProgram(const char *fileVS, const char *fileFS);
    bool setShaderProgram(int index);
    void moveCamera(int id);

  private:
    std::string ErrorString(GLenum error);
   
    Camera *m_camera;
    Shader *m_shader;
    std::vector<Shader*> m_shaderArr;

    GLint l_amb;
    GLint l_dif;
    GLint l_spec;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint l_lightPos;
    GLint l_shininess;

    GLint l_dimness;
    GLint l_ambDimness;

    GLint gSampler;

    Object *board;
    Object *lFlipper;
    Object *rFlipper;
    Object *bumperOne;
    Object *bumperTwo;
    Object *bumperThree;
    Object *ball;

    glm::vec4 lightPos;
    float g_dimness, g_ambDimness;

    //collision variables
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    

    bool clockwise = true;

    float SLOPE;
};

#endif /* GRAPHICS_H */
