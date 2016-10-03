#include "object.h"

Object::Object(float oRadius, float oSpeed)
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }


init(oRadius, oSpeed);
}

Object::Object(float oRadius, float oSpeed, std::string objPath )
{

//variables and initialization

Assimp::Importer importer;


//read obj file
const aiScene *object = importer.ReadFile( objPath, aiProcess_Triangulate );
aiMesh *mesh = object->mMeshes[0];

/*


insert code for reading stuff


*/

init(oRadius, oSpeed);

}


void Object::init( float oRadius, float oSpeed )
{


  rotAngle = 0.0f;
  orbitAngle = 0.0f;
  orbitRadius = oRadius;
  orbitSpeed = oSpeed;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}


Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, float movement[], bool pause)
{
  
  //module for orbit and rotation response to controls
  if( !pause ){
    if( glm::abs(movement[0]) < 2.0f )
    {
      rotAngle += dt * (M_PI/1000) * movement[0];
    }

    if( glm::abs(movement[1]) < 2.0f )
    {
      orbitAngle += dt * (M_PI/10000) * movement[1] * orbitSpeed;  
    }
  }

  //specified variables distX and distZ to specify translation of cube in orbit
  float distX = orbitRadius * glm::cos(orbitAngle);

  float distZ = orbitRadius * glm::sin(orbitAngle);
   
  //return cube back to origin
  model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,0.0));

  //move cube to location specified distX and distZ
  model = glm::translate(model, glm::vec3(distX,0.0,distZ));

  //rotate cube
  model = glm::rotate( model, (rotAngle), glm::vec3(0.0, 1.0, 0.0));

}
glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

