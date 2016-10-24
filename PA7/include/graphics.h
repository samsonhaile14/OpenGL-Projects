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
    Graphics(std::vector< Planet > solSys);
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(unsigned int dt, float timeScale, float movement[], bool pause, bool isZoomToggled, float zoomFactor);
    void Render();
    void UpdateCamera(glm::vec3);
    void goToPlanet(int);
    void resetCamera();
    float getZoomFactor(int planetIndex);

  private:
    std::string ErrorString(GLenum error);
   
    std::vector< Planet > setting;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint gSampler;

    std::vector< Object > pObjs;
    std::vector< vector< Object > > mObjs;


    bool clockwise = true;
};

#endif /* GRAPHICS_H */
