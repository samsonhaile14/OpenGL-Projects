#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>
#include <vector>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

struct Sphere
{

std::string name;
std::string textureFile;

float diameter;
float orbitRadius;
float orbitSpeed;
float rotationPeriod;
float zoomFactor;

bool hasRing;

};

struct Planet
{

Sphere pAttr;
std::vector<Sphere> moons;

};

struct Vertex
{
  glm::vec3 vertex;
  glm::vec2 tex;

  Vertex(glm::vec3 v, glm::vec2 t): vertex(v), tex(t) {}
};

#endif /* GRAPHICS_HEADERS_H */
