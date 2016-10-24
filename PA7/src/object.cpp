#include "object.h"


Object::Object(Sphere setting, std::string objPath, std::string texturePath)
{
  //variables and initialization

  Assimp::Importer importer;

  textureFileName = texturePath;

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
//       aiString texFileName;
       Magick::Blob m_blob;
       GLuint texObj;
//       aiReturn texFound = material->GetTexture( aiTextureType_DIFFUSE, 0, &texFileName );

       try {
          Magick::Image skin;
//          std::string rStr = texFileName.C_Str();
          std::string texFile = textureFileName;
          skin.read( texFile );
          skin.write( &m_blob, "RGBA");
          blobs.push_back(m_blob);

          skins.push_back( skin );
       }
       catch (Magick::Error& Error) {
         std::cout << "Error loading texture '" << textureFileName.c_str() << "': " << 
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

  init(setting);
}


void Object::init( Sphere setting )
{

  rotAngle = 0.0f;
  orbitAngle = 0.0f;

  specs = setting;

  //rescale attributes to be suitable for display
  //multipliers in parentheses are scale factors for visibility


  specs.diameter *= (30.0f);
  specs.orbitRadius *= 1000000.0 / (250.0); //multiplied by 1000 to make up for 10^6 difference in config file

   //convert linear speed to rotational speed
   if( specs.orbitSpeed != 0 && specs.orbitRadius != 0 ){
     specs.orbitSpeed = (specs.orbitSpeed) / (2.0 * (M_PI) * specs.orbitRadius); //user scalable
     }

  //specs.rotationPeriod is user scalable

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

void Object::Update(unsigned int dt, float timeScale, float movement[], bool pause, bool isZoomToggled, float zf)
{

  //module for orbit and rotation response to controls
  if( !pause ){
    if( glm::abs(movement[0]) < 2.0f )
    {

      if( specs.rotationPeriod != 0 ){
         rotAngle += ( (float)(dt) / 1000.0f ) * (M_PI) * movement[0] * (timeScale/ (specs.rotationPeriod * 86400.0) );
         }
    }

    if( glm::abs(movement[1]) < 2.0f )
    {
      orbitAngle += ( ((float) (dt)) / 1000.0f ) * (M_PI) * movement[1] * timeScale * specs.orbitSpeed;
    }
  }

  //specified variables distX and distZ to specify translation of cube in orbit
  distX = specs.orbitRadius * glm::cos(orbitAngle);

  distZ = specs.orbitRadius * glm::sin(orbitAngle);

  //return cube back to origin
  float ratio = specs.diameter;


  //move cube to location specified distX and distZ
  if( isZoomToggled )
  model = glm::translate(glm::mat4(1.0), glm::vec3(distX,0.0,distZ + zf ));
  else
  model = glm::translate(glm::mat4(1.0), glm::vec3(distX,0.0,distZ));
  model = glm::scale( model, glm::vec3(ratio, ratio, ratio) );

  //rotate cube
  model = glm::rotate( model, 3.1f, glm::vec3(1.0, 0.0, 0.0));
  if( specs.rotationPeriod != 0 ){
     model = glm::rotate( model, (rotAngle), glm::vec3(0.0, 1.0, 0.0));
  }

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

// function: obtain position of center of the object
glm::vec3 Object::getPosition(){
   return glm::vec3(distX, distY, distZ);
}

float Object::getZoomFactor(){
  return specs.zoomFactor;
}
