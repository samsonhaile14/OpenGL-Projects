#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, 80.0, -10.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up


  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}

void Camera::setView(int id){

  switch(id){

    // top-down
    case 1:
      view = glm::lookAt( glm::vec3(0.0, 80.0, -10.0), //Eye Position
                          glm::vec3(0.0, 0.0, 0.0), //Focus point
                          glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
      break;

    // normal
    case 2:
      view = glm::lookAt( glm::vec3(0.0, 30.0, -30.0), //Eye Position
                          glm::vec3(0.0, 10.0, 5.0), //Focus point
                          glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
      break;

    // close-up
    case 3:
      view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                          glm::vec3(0.0, 0.0, 0.0), //Focus point
                          glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
      break;

    // bystander view
    case 4:
      view = glm::lookAt( glm::vec3(20.0, 20.0, -20.0), //Eye Position
                          glm::vec3(0.0, 2.0, 15.0), //Focus point
                          glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
      break;

    // plunger view
    case 5:
      view = glm::lookAt( glm::vec3(-10.0, 20.0, -25.0), //Eye Position
                          glm::vec3(-10.0, -5.0, 10.0), //Focus point
                          glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
      break;

    default:
      printf("Error: Bad input. Cannot change camera angle.\r\n");
      break;
  }
}
