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
    void Update(glm::vec3);
    void goToPlanet(glm::vec3);
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 eyePosition = glm::vec3(0.0, 8.0, -16.0);
};

#endif /* CAMERA_H */
