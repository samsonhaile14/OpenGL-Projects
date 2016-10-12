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
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f}}
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

Object::Object(float oRadius, float oSpeed, std::string objPath)
{
  //variables and initialization

  Assimp::Importer importer;


  //read obj file
  const aiScene *object = importer.ReadFile( objPath, aiProcess_Triangulate |         
                                           aiProcess_JoinIdenticalVertices );

  //get texture file name

  // loop through meshes
  int totalMeshVerts = 0;
  for( int i = 0; i < object->mNumMeshes; ++i ){

    // get mesh array
    aiMesh *mesh = object->mMeshes[i];

    // get face values
    indicesStart.push_back( Indices.size());
    for( int j = 0; j < mesh->mNumFaces; ++j ){
      aiFace face = mesh->mFaces[j];
      for( int k = 0; k < face.mNumIndices; ++k ){
        Indices.push_back( face.mIndices[k] + totalMeshVerts );
      }

    }
    indicesSize.push_back( Indices.size() - indicesStart[i]);

    // get color value
    unsigned int mtlIndex = mesh->mMaterialIndex;
    aiMaterial *material = object->mMaterials[mtlIndex]; 

    //set up texture   
       aiString texFileName;
       Magick::Blob m_blob;
       GLuint texObj;
       aiReturn texFound = material->GetTexture( aiTextureType_DIFFUSE, 0, &texFileName );

       try {
          Magick::Image skin;
          std::string rStr = texFileName.C_Str();
          skin.read( "../objects/" + rStr );
          skin.write( &m_blob, "RGBA");
          blobs.push_back(m_blob);

          skins.push_back( skin );
       }
       catch (Magick::Error& Error) {
         std::cout << "Error loading texture '" << texFileName.C_Str() << "': " << 
                      Error.what() << std::endl;
         return;
       }

       glGenTextures( 1, &texObj );
       texObjs.push_back( texObj );

       glBindTexture(GL_TEXTURE_2D,texObj);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, skins[skins.size()-1].columns(), 
                     skins[skins.size()-1].rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                     m_blob.data());

       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
       glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // read vertex values and store in VBO
    for( int j = 0; j < mesh->mNumVertices; ++j ){
      aiVector3D vertex = mesh->mVertices[j];

      aiVector3D uv = mesh->mTextureCoords[0][j];

      Vertices.push_back( {
        {vertex.x,vertex.y,vertex.z},
        {uv.x,uv.y}
      });
    }

  totalMeshVerts = Vertices.size();
  }

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

void Object::Render(GLint gSampler)
{

  int indx;

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);

  for( indx = 0; indx < texObjs.size(); indx++ ){

     glUniform1i(gSampler,indx);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

     Bind(GL_TEXTURE0 + indx, indx);

     glDrawElements(GL_TRIANGLES, indicesSize[indx], GL_UNSIGNED_INT, 
                    (void*) (indicesStart[indx]*sizeof(unsigned int)));

  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void Object::Bind(GLenum TextureUnit, int texIndx){
   glActiveTexture(TextureUnit);
   glBindTexture(GL_TEXTURE_2D, texObjs[texIndx] );
}


