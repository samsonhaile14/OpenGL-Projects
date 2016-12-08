#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    void setView(float x,float y);
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    float r;

    float turnFactor;

    float turnDirX,turnDirY;

    glm::vec3 dir,up,horAxis;
    glm::vec3 center;
};

#endif /* CAMERA_H */
