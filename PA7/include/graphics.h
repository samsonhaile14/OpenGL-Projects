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
    void Update(unsigned int dt, float movement[], bool pause);
    void Render();

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
