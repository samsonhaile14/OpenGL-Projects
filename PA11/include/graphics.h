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
    void moveCamera(float x,float y);
    void addAmbientDim(float val);
    void addSpecularDim(float val);
    void adjustSpotlightRadius(float val);
    void adjustSpotlightIntensity(float val);
    void resetGame();

    void movePlayer(int direction);

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
    GLint l_lightPosB;
    GLint l_lightPosC;
    GLint l_shininess;

    GLint l_dimness;
    GLint l_ambDimness;
    GLint l_specDimness;

    GLint l_cutoffAngle;
    GLint l_spotlightIntensity;

    GLint gSampler;

    Object *board;
    Object *player;

    glm::vec4 lightPos;
    glm::vec4 lightPosB;
    glm::vec4 lightPosC;
    float g_dimness, g_ambDimness;
    float g_specDimness;
    float g_cutoffAngle;
    float g_spotlightIntensity;

    //collision variables
    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;
    

    bool clockwise = true;

    float SLOPE;

    int score;
    int lives;
    bool isGameOver;
};

#endif /* GRAPHICS_H */
