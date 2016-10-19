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
  view = glm::lookAt( eyePosition, //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 10000.0f); //Distance to the far plane, 
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

// function: set camera to provided location, facing world origin
void Camera::Update(glm::vec3 pos){

  // set eye position and move camera there
  eyePosition = pos;
  view = glm::lookAt( eyePosition,
                      glm::vec3(0.0,0.0,0.0),
                      glm::vec3(0.0,1.0,0.0));
}


// function: move camera towards planet, given planet coordinates
void Camera::goToPlanet(glm::vec3 pos){

  // have camera face towards center of the planet
  glm::vec3 facing = glm::vec3(pos);

  // move camera position just outside of center location
  eyePosition = glm::vec3(pos);
  eyePosition.z += 5.0f;

  // set camera
  view = glm::lookAt( eyePosition,
                      facing,
                      glm::vec3(0.0,1.0,0.0));
}
