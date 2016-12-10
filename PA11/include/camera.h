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
    void updateView();
    void setView(float x,float y);

    glm::vec3 center;  
    float r;
    glm::vec3 dir,up,horAxis;


  private:
    glm::mat4 projection;
    glm::mat4 view;

    float turnFactor;

    float turnDirX,turnDirY;

};

#endif /* CAMERA_H */
