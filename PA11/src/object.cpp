#include "object.h"

Object::Object(float x, float y, float z,float rx, float ry, float rz,float m_mass,int meshType, std::string objPath)
{
  //variables and initialization
  globalPos.x = x;
  globalPos.y = y;
  globalPos.z = z;
  globalRot.x = rx;
  globalRot.y = ry;
  globalRot.z = rz;

  Assimp::Importer importer;

  objTriMesh = new btTriangleMesh();

  //read obj file
  const aiScene *object = importer.ReadFile( objPath, aiProcess_Triangulate |         
                                           aiProcess_JoinIdenticalVertices );

  //get texture file name

  // loop through meshes
  int totalMeshVerts = 0;
  for( unsigned int i = 0; i < object->mNumMeshes; ++i ){

    // get mesh array
    aiMesh *mesh = object->mMeshes[i];

    // get color value
    unsigned int mtlIndex = mesh->mMaterialIndex;
    aiMaterial *material = object->mMaterials[mtlIndex]; 

    //record material values
      aiColor3D surfValues;
      material->Get( AI_MATKEY_COLOR_AMBIENT, surfValues );
      ambient.push_back( glm::vec4( surfValues.r,surfValues.g,surfValues.b, 1.0 ) );

      material->Get( AI_MATKEY_COLOR_DIFFUSE, surfValues );      
      diffuse.push_back( glm::vec4( surfValues.r,surfValues.g,surfValues.b, 1.0 ) );

      material->Get( AI_MATKEY_COLOR_SPECULAR, surfValues );
      specular.push_back( glm::vec4( surfValues.r,surfValues.g,surfValues.b, 1.0 ) );

      material->Get( AI_MATKEY_SHININESS, shininess );
      shininess/=1024.0;

    //set up texture   
       aiString texFileName;
       Magick::Blob m_blob;
       GLuint texObj;
       aiReturn texFound = material->GetTexture( aiTextureType_DIFFUSE, 0, &texFileName );
       if( texFound != 0 )
         printf("An error occured while getting textures. Code: %d\r\n", texFound);

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
    for( unsigned int j = 0; j < mesh->mNumVertices; ++j ){
      aiVector3D vertex = mesh->mVertices[j];

      aiVector3D uv = mesh->mTextureCoords[0][j];

      aiVector3D nm = mesh->mNormals[j];
      Vertices.push_back( {
        {vertex.x,vertex.y,vertex.z},
        {uv.x,uv.y},
        {nm.x,nm.y,nm.z}
      });
    }

    // get face values
    indicesStart.push_back( Indices.size());
    for( unsigned int j = 0; j < mesh->mNumFaces; ++j ){

      aiFace face = mesh->mFaces[j];
      btVector3 triArray[3];

      for( unsigned int k = 0; k < face.mNumIndices; ++k ){
        
        Indices.push_back( face.mIndices[k] + totalMeshVerts );
        triArray[k] = btVector3(Vertices[face.mIndices[k]+totalMeshVerts].vertex.x,
                                Vertices[face.mIndices[k]+totalMeshVerts].vertex.y,
                                Vertices[face.mIndices[k]+totalMeshVerts].vertex.z );        
      }

      objTriMesh->addTriangle( triArray[0], triArray[1], triArray[2] );

    }
    indicesSize.push_back( Indices.size() - indicesStart[i]);


  totalMeshVerts = Vertices.size();
  }

  init(m_mass,meshType);
}


void Object::init(float m_mass,int meshType)
{

  //create collision shape and set properties
  if(meshType==1){
     shape = new btSphereShape(1.0);
  }

  else if(meshType==2){
     shape = new btCylinderShapeZ(btVector3(1.0,0.5,0.5 ));
  }

  else if(meshType==4){
     shape = new btBoxShape(btVector3(1.0,2.0,0.2));
   }

  else if( objTriMesh != NULL ){
     shape = new btBvhTriangleMeshShape(objTriMesh, true);
  }
  else{
   printf("error\n");
   return;
  }

  btQuaternion rot;
  rot.setEulerZYX( globalRot.z, globalRot.y,globalRot.x );

  shapeMotionState = new btDefaultMotionState(
                      btTransform(rot,btVector3(globalPos.x,globalPos.y,globalPos.z)));

  mass = m_mass;
  inertia = btVector3(1.0,1.0,1.0);
  shape->calculateLocalInertia(mass,inertia);
  btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass,shapeMotionState,
                                                            shape,inertia);

   rigidBody = new btRigidBody(shapeRigidBodyCI);
   rigidBody->setMotionState(shapeMotionState); //constructor doesn't properly set motionstate

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

}


Object::~Object()
{

  if(objTriMesh != NULL){
   delete objTriMesh;
   objTriMesh = NULL;
  }

   if(shape != NULL){
    delete shape;
    shape = NULL;
   }

   if( shapeMotionState != NULL){
    delete shapeMotionState;
    shapeMotionState = NULL;
   }

   if( rigidBody != NULL ){
    delete rigidBody;
    rigidBody = NULL;
   }

  Vertices.clear();
  Indices.clear();

  skins.clear();
  blobs.clear();
  texObjs.clear();
}

void Object::Update(unsigned int dt, float movement[], bool pause)
{     
btTransform trans;
btScalar m[16];

rigidBody->getMotionState()->getWorldTransform(trans);

trans.getOpenGLMatrix(m);

model = glm::make_mat4(m);
}


glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render(GLint gSampler, GLint gAmb, GLint gDif, GLint gSpe, GLint shine)
{

  unsigned int indx;

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

  for( indx = 0; indx < texObjs.size(); indx++ ){

     //for texture loading
        glUniform1i(gSampler,indx);

     //for reporting surface properties
        glUniform4fv( gAmb, 1, glm::value_ptr(ambient[indx]) );
        glUniform4fv( gDif, 1, glm::value_ptr(diffuse[indx]) );
        glUniform4fv( gSpe, 1, glm::value_ptr(specular[indx]) );
        glUniform1f( shine, shininess );

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

     Bind(GL_TEXTURE0 + indx, indx);

     glDrawElements(GL_TRIANGLES, indicesSize[indx], GL_UNSIGNED_INT, 
                    (void*) (indicesStart[indx]*sizeof(unsigned int)));

  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

void Object::Bind(GLenum TextureUnit, int texIndx){
   glActiveTexture(TextureUnit);
   glBindTexture(GL_TEXTURE_2D, texObjs[texIndx] );
}

void Object::setModel(glm::mat4 m){
  model = m;
}
