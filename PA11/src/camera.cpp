#include "camera.h"
#include <cmath>

Camera::Camera()
{
r = 2.0;
dir = glm::vec3(0.0,0.0,r);
center = glm::vec3(0.0,3.0,0.0);

turnFactor = 0.05;
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
  view = glm::lookAt( glm::vec3(0.0, 80.0,-10.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up


  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 200.0f); //Distance to the far plane, 
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

glm::mat4 rotation;
glm::vec4 result;

//set rotation
   switch(id){
      case 1:
         rotation = glm::rotate(glm::mat4(1.f), turnFactor, glm::vec3(0.0,1.0,0.0));
         break;
      
      case 2:
         rotation = glm::rotate(glm::mat4(1.f), (-1.0f) * turnFactor, glm::vec3(0.0,1.0,0.0));
         break;

      case 3:
         rotation = glm::rotate(glm::mat4(1.f), turnFactor, glm::vec3(1.0,0.0,0.0));
         break;

      case 4:
         rotation = glm::rotate(glm::mat4(1.f), (-1.0f) * turnFactor, glm::vec3(1.0,0.0,0.0));
         break;

   }

//set result
   result = rotation * glm::vec4(dir,1.0);
   dir = glm::vec3(result.x,result.y,result.z);

//change view
   view = glm::lookAt( center + dir, //Eye Position
                       center + dir + dir, //Focus point
                       glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

}
