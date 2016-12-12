#include "graphics.h"

#define BIT(x) (1<<(x))

enum collisionTypes{

   COL_NOTHING = 0,
   COL_WALL = BIT(1),
   COL_BALL = BIT(2)
};

Graphics::Graphics(float dimness, float ambDimness )
{

g_dimness = dimness;
g_ambDimness = ambDimness;
g_specDimness = 1.0;
g_cutoffAngle = 0.65;
g_spotlightIntensity = 2.0;
score = 0;
lives = 4;
isGameOver = false;

lvEnd[0] = glm::vec3( -557.6586,25.0000,297.7418 );
lvEnd[1] = glm::vec3( -593.3218,-78.1196,84.5833 );
lvEnd[2] = glm::vec3(67.3005, 137.1841, 119.9242);


lightPosC = glm::vec4(lvEnd[levelNumber - 1],1.0);

}

Graphics::~Graphics()
{
 
  //bullet stuff 
  if(broadphase != NULL ){
   delete broadphase;
   broadphase = NULL;
  }

  if(collisionConfiguration != NULL ){
   delete collisionConfiguration;
   collisionConfiguration = NULL;
  }

  if(dispatcher != NULL){
   delete dispatcher;
   dispatcher = NULL;
  }

  if(solver != NULL){
   delete solver;
   solver = NULL;
  }

  if(dynamicsWorld != NULL){
   //delete dynamicsWorld;
   //dynamicsWorld = NULL;
  }

  //objects
    if( board != NULL ){
     delete board;
     board = NULL;
    }

    if( player != NULL ){
     delete player;
     player = NULL;
    }

    if( enemies != NULL ){
      for( int i = 0; i < numEnemies; ++i ){
        if( enemies[i] != NULL ){
          delete enemies[i];
          enemies[i] = NULL;
        }
      }
      delete[] enemies;
      enemies = NULL;
    }

    if( enemyDirection != NULL ){
      delete[] enemyDirection;
      enemyDirection = NULL;
    }

    if( enemyBullets != NULL ){
      for( int i = 0; i < numEnemies; ++i ){

        if( enemyBullets[i] != NULL ){
          for( int j = 0; j < numBulletsPerEnemy; ++j ){

            if( enemyBullets[i][j] != NULL ){
              delete enemyBullets[i][j];
              enemyBullets[i][j] = NULL;
            }
          }

          delete[] enemyBullets[i];
          enemyBullets[i] = NULL;
        }
      }

      delete[] enemyBullets;
      enemyBullets = NULL;
    }

    if( isBulletUsed != NULL ){
      delete[] isBulletUsed;
      isBulletUsed = NULL;
    }
}

bool Graphics::Initialize(int width, int height)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create collider environment
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver();
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, 
                                              collisionConfiguration);

  // Create the objects

   std::string path = "../objects/level";
   path += ('0'+levelNumber);
   path +=".obj";
   board = new Object(0.0,-1.0,0.0, //position
                      0.0,(2.0 * 3.141592) / 2.0,0.0 , //rotation
                     0.0, 0,path.c_str());   //mass,meshtype,objfile
   player = new Object(0.0,12.0,0.0, //position
                      0.0,(2.0 * 3.141592) / 4.0,0.0 , //rotation
                     1.0, 1,"../objects/sphere.obj");   //mass,meshtype,objfile

   glm::vec3 playerPos = glm::vec3(
                           player->rigidBody->getCenterOfMassPosition().getX(),
                           player->rigidBody->getCenterOfMassPosition().getY(),
                           player->rigidBody->getCenterOfMassPosition().getZ()
                          );

   glm::vec3 enemyPos = glm::vec3(0.0, 8.0, -17.0);
   enemies = new Object*[numEnemies];
   enemyDirection = new glm::vec3[numEnemies];
   isBulletUsed = new bool[numEnemies];
   for( int i = 0; i < numEnemies; ++i ){
     enemies[i] = new Object(enemyPos.x, enemyPos.y, enemyPos.z,
                              0.0, 0.0, 0.0,
                              0.0, 4, "../objects/player.obj");
     
     facePlayer(i, playerPos);
     isBulletUsed[i] = false;
   }

   enemyBullets = new Object**[numEnemies];
   for( int i = 0; i < numEnemies; ++i ){ 
     enemyBullets[i] = new Object*[numBulletsPerEnemy];
     for( int j = 0; j < numBulletsPerEnemy; ++j ){
  
       enemyBullets[i][j] = new Object(enemyPos.x+3*i, -15.0, enemyPos.z+3*j,
                              0.0, 0.0, 0.0,
                              1.0, 1, "../objects/sphere.obj");
     }
   }


  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  dynamicsWorld->setGravity(btVector3(0.0,-2.0,0));

  //Set kinematic/static rigidbodies and add rigidbodies to dynamicWorld

  int ballCollide = COL_WALL;

  dynamicsWorld->addRigidBody(board->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(player->rigidBody,COL_BALL,ballCollide);

  dynamicsWorld->addRigidBody(player->rigidBody,COL_BALL,COL_WALL);
  for( int i = 0; i < numEnemies; ++i ){
    dynamicsWorld->addRigidBody(enemies[i]->rigidBody,COL_BALL,COL_WALL);
  }

  for( int i = 0; i < numEnemies; ++i ){
    for( int j = 0; j < numBulletsPerEnemy; ++j ){
//      dynamicsWorld->addRigidBody(enemyBullets[i][j]->rigidBody,COL_BALL,ballCollide);
      dynamicsWorld->addRigidBody(enemyBullets[i][j]->rigidBody, COL_BALL, COL_WALL);
    }
  }


  board->rigidBody->setCollisionFlags(board->rigidBody->getCollisionFlags() | 
                                      btCollisionObject::CF_KINEMATIC_OBJECT);
  board->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  player->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  for( int i = 0; i < numEnemies; ++i ){
    (enemies[i])->rigidBody->setActivationState(DISABLE_DEACTIVATION);
  }

  for( int i = 0; i < numEnemies; ++i ){
    for( int j = 0; j < numBulletsPerEnemy; ++j ){
      enemyBullets[i][j]->rigidBody->setActivationState(DISABLE_DEACTIVATION);
    }
  }

  // Set up the shaders
  if( !loadShaderProgram("vl_vertex.glsl", "vl_fragment.glsl") ){
    printf("Fail to load shader program (0)\r\n");
    return false;
  }

  if( !loadShaderProgram("fl_vertex.glsl", "fl_fragment.glsl") ){
    printf("Fail to load shader program (1)\r\n");
    return false;
  }

  // load default shader
  if( !setShaderProgram(0) ){
    printf("Setting shader failed.\r\n");
    return false;
  }

  //set light position
   lightPos = glm::vec4( 0.0,50.0,0.0,1.0 );

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, float movement[])
{
  dynamicsWorld->stepSimulation(dt,5);

  // board update
  board->Update( dt, movement, false);

  // if player drops off of map, set back to position
     if( player->rigidBody->getCenterOfMassPosition().y() < -100.f ){
       btTransform pos = player->rigidBody->getCenterOfMassTransform();
       pos.setOrigin( btVector3(0.0,12.0,0.0) );
       player->rigidBody->setCenterOfMassTransform(pos);
       player->rigidBody->setLinearVelocity(btVector3(0.0,0.0,0.0));
      }

  // player and camera update
     player->Update( dt, movement, false);
     m_camera->center.x = player->rigidBody->getCenterOfMassPosition().getX();
     m_camera->center.y = player->rigidBody->getCenterOfMassPosition().getY() + 3.0;
     m_camera->center.z = player->rigidBody->getCenterOfMassPosition().getZ();
     m_camera->updateView();

<<<<<<< HEAD
//  printf( "%f %f %f\n", -illumPlayer.x,-illumPlayer.y,-illumPlayer.z);
  lightPosB = glm::vec4(-illumPlayer.x,illumPlayer.y + 10.0,-illumPlayer.z,1.0);
=======
  //spotlight over player
     glm::mat4 pMat = player->GetModel();
     glm::vec4 illumPlayer = glm::vec4(pMat[3]);
     lightPosB = glm::vec4(-illumPlayer.x,illumPlayer.y + 10.0,-illumPlayer.z,1.0);
>>>>>>> dbfaaf763b6371c83677b9bf657b6fdc1ea0002d
   

<<<<<<< HEAD
  // enemy update
  glm::vec3 playerPos = glm::vec3(
                           player->rigidBody->getCenterOfMassPosition().getX(),
                           player->rigidBody->getCenterOfMassPosition().getY(),
                           player->rigidBody->getCenterOfMassPosition().getZ()
                          );
  for( int i = 0; i < numEnemies; ++i ){
    
    // enemy direction
    facePlayer(i, playerPos);

    enemies[i]->Update( dt, movement, false);
  }

  // enemy bullet update
  for( int i = 0; i < numEnemies; ++i ){
    for( int j = 0; j < numBulletsPerEnemy; ++j ){
      setBulletMotion(i);
      enemyBullets[i][j]->Update( dt, movement, false);
   
      // check if bullet collided
      numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
      for( int k = 0; k < numManifolds; ++k ){
          btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(k);


          int numContacts = contactManifold->getNumContacts();
          for( int l = 0; l < numContacts; ++l){
            btManifoldPoint &pt = contactManifold->getContactPoint(l);
            if( pt.getDistance() < 0.f){
              const btCollisionObject *obA = contactManifold->getBody0();
              const btTransform transA = obA->getWorldTransform();
              const btVector3 originA = transA.getOrigin();

              const btCollisionObject *obB = contactManifold->getBody1();
              const btTransform transB = obB->getWorldTransform();
              const btVector3 originB = transB.getOrigin();
        
              if( originA == enemyBullets[i][j]->rigidBody->getCenterOfMassPosition() ||
                 originB == enemyBullets[i][j]->rigidBody->getCenterOfMassPosition() )
              {
                resetBullet(enemyBullets[i][j], i*numEnemies+j);
              } 
            }
          }
        }
      }
   }
=======
   //record time since last jump
      playerOnGround += float(dt)/1000.f;

   //if player completes level, progress to next
      glm::vec4 playerPos = lightPosB;
      playerPos.y += 10.0;
      float dist = (playerPos.x - lightPosC.x) * (playerPos.x - lightPosC.x) +
                   (playerPos.z - lightPosC.z) * (playerPos.z - lightPosC.z) +
                   (playerPos.y - lightPosC.y) * (playerPos.y - lightPosC.y);
      dist = sqrt( dist );
      printf("%f vs. %f, %f vs. %f, %f vs. %f\n",playerPos.x,lightPosC.x, playerPos.y,lightPosC.y, playerPos.z,lightPosC.z);
      if( dist < 10.0){
         if(levelNumber < 3){
            printf("congratulations, moving on to next level\n");
            levelNumber++;
            //removing level from scene and creating next
               dynamicsWorld->removeRigidBody(board->rigidBody);
               delete board;
               std::string path = "../objects/level";
               path += ('0'+levelNumber);
               path +=".obj";
               board =    new Object(0.0,-1.0,0.0, //position
                      0.0,(2.0 * 3.141592) / 2.0,0.0 , //rotation
                     0.0, 0,path.c_str() );   //mass,meshtype,objfile

               dynamicsWorld->addRigidBody(board->rigidBody,COL_WALL,COL_BALL);
               board->rigidBody->setCollisionFlags(board->rigidBody->getCollisionFlags() | 
                                                     btCollisionObject::CF_KINEMATIC_OBJECT);
               board->rigidBody->setActivationState(DISABLE_DEACTIVATION);
            //replacing ball at start
                btTransform pos = player->rigidBody->getCenterOfMassTransform();
                pos.setOrigin( btVector3(0.0,12.0,0.0) );
                player->rigidBody->setCenterOfMassTransform(pos);
                player->rigidBody->setLinearVelocity(btVector3(0.0,0.0,0.0));
            //changing goal point
               lightPosC = glm::vec4(lvEnd[levelNumber-1],1.0);
               
         }

         else{
            printf("congratulations, you have beat all the levels!\n" );
            
         }
      }
      
>>>>>>> dbfaaf763b6371c83677b9bf657b6fdc1ea0002d
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  //set light position and brightness
  glUniform4fv( l_lightPos, 1, glm::value_ptr(lightPos) );
  glUniform4fv( l_lightPosB, 1, glm::value_ptr(lightPosB) );
  glUniform4fv( l_lightPosC, 1, glm::value_ptr(lightPosC) );
  glUniform1f( l_dimness, g_dimness );
  glUniform1f( l_ambDimness, g_ambDimness );
  glUniform1f( l_specDimness, g_specDimness );
  glUniform1f( l_cutoffAngle, g_cutoffAngle );
  glUniform1f( l_spotlightIntensity, g_spotlightIntensity );

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
  board->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(player->GetModel()));
  player->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  for( int i = 0; i < numEnemies; ++i ){
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(enemies[i]->GetModel()));
    enemies[i]->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);
  }

  for( int i = 0; i < numEnemies; ++i ){
    for( int j = 0; j < numBulletsPerEnemy; ++j ){
      glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(enemyBullets[i][j]->GetModel()));
      enemyBullets[i][j]->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);
    }
  }

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

bool Graphics::loadShaderProgram(const char *fileVS, const char *fileFS){

  Shader *newProgram = new Shader();
  if(!newProgram->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!newProgram->AddShader(GL_VERTEX_SHADER, fileVS))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!newProgram->AddShader(GL_FRAGMENT_SHADER, fileFS))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!newProgram->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  m_shaderArr.push_back(newProgram);
  return true;
}

bool Graphics::setShaderProgram(int index){

  // error check
  if( index < 0 || (unsigned int)index >= m_shaderArr.size() ){
    printf("Error: shader list index %d out of range.\r\n", index);
    return false;
  }

  // set new shader
  m_shader = m_shaderArr[index];

  // reassignn uniform locations
  l_amb = m_shader->GetUniformLocation("AmbientProduct");
  l_dif = m_shader->GetUniformLocation("DiffuseProduct");
  l_spec = m_shader->GetUniformLocation("SpecularProduct");

  l_shininess = m_shader->GetUniformLocation("Shininess");
  l_lightPos = m_shader->GetUniformLocation("LightPosition");
  l_lightPosB = m_shader->GetUniformLocation("LightPositionB");
  l_lightPosC = m_shader->GetUniformLocation("LightPositionC");


  l_specDimness = m_shader->GetUniformLocation("SpecularDimness");

  l_dimness = m_shader->GetUniformLocation("Dimness");
  l_ambDimness = m_shader->GetUniformLocation("AmbientDimness");
  
  l_cutoffAngle = m_shader->GetUniformLocation("CutoffAngle");
  l_spotlightIntensity = m_shader->GetUniformLocation("SpotlightIntensity");

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION) 
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  gSampler = m_shader->GetUniformLocation( "gSampler" );


  return true;
}

void Graphics::moveCamera(float x,float y){
  m_camera->setView(x,y);
}

void Graphics::addAmbientDim(float val){
  if( g_ambDimness+val > 0.00 && g_ambDimness+val < 3.00 )
    g_ambDimness += val;
}

void Graphics::addSpecularDim(float val){
  if( g_specDimness+val > 0.00 && g_specDimness+val < 3.00 )
    g_specDimness += val;
}

void Graphics::adjustSpotlightRadius(float val){
  if( g_cutoffAngle+val > 0.05 && g_cutoffAngle+val < 0.95 )
    g_cutoffAngle += val;
}

void Graphics::adjustSpotlightIntensity(float val){
  if( g_spotlightIntensity+val > 0.05 && g_spotlightIntensity+val < 2.00 )
    g_spotlightIntensity += val;
}

void Graphics::resetGame(){

}

void Graphics::movePlayer(int direction){

  glm::vec3 force;
  float multiplier = 1.0;

  switch(direction){

    // up
    case 0:
      force = (m_camera->dir);
      force.x *= multiplier/m_camera->r;
      force.y = 0;
      force.z *= multiplier/m_camera->r;
      player->rigidBody->applyCentralForce( btVector3(force.x,force.y,force.z) );
      break;

    // down
    case 1:
      force = (m_camera->dir);
      force.x *= -multiplier/m_camera->r;
      force.y = 0;
      force.z *= -multiplier/m_camera->r;
      player->rigidBody->applyCentralForce( btVector3(force.x,force.y,force.z) );
      break;

    // left
    case 2:
      force = (m_camera->horAxis);
      force.x *= multiplier/m_camera->r;
      force.y = 0;
      force.z *= multiplier/m_camera->r;
      player->rigidBody->applyCentralForce( btVector3(force.x,force.y,force.z) );
      break;

    // right
    case 3:
      force = (m_camera->horAxis);
      force.x *= -multiplier/m_camera->r;
      force.y = 0;
      force.z *= -multiplier/m_camera->r;
      player->rigidBody->applyCentralForce( btVector3(force.x,force.y,force.z) );
      break;

    //jump
    case 4:
      if(playerOnGround >= 3.0){
         player->rigidBody->applyCentralForce( btVector3(0,100.0,0.0) );
         jumps++;
         if(jumps >= 2){
            playerOnGround = 0;
            jumps = 0;
         }
      }
      break;

    //halt ball
    case 5:
         player->rigidBody->setLinearVelocity( btVector3(0,0,0) );       
         break;

  }
  return;
}

void Graphics::facePlayer(int enemyID, glm::vec3 playerPos){
  Object *enemy = enemies[enemyID];

  // get angle from default (0,1) to direction of player (on XY-plane)
    // get positions
  glm::vec2 playerPosHoriz = glm::vec2( playerPos.x, playerPos.z );

  glm::vec2 enemyPosHoriz = glm::vec2(
                          enemy->rigidBody->getCenterOfMassPosition().getX(),
                          enemy->rigidBody->getCenterOfMassPosition().getZ()
                         );

    // get normalized vector from positions
  glm::vec2 distPE;
  distPE.x = playerPosHoriz.x - enemyPosHoriz.x;
  distPE.y = playerPosHoriz.y - enemyPosHoriz.y;
  distPE = normalize(distPE);

  // set enemy direction
  enemyDirection[enemyID] = glm::vec3(distPE.x, 0.0, distPE.y); // dummy

    // get angle of rotation (dot product with vector (0,1))
  float angle = distPE.y;
  angle = acos(angle);
  if( playerPosHoriz.x - enemyPosHoriz.x <= 0 )
    angle *= -1;


  // set angle
    // get current state 
  btMotionState *newState = enemy->rigidBody->getMotionState();
  btTransform newTrans;
  newState->getWorldTransform(newTrans);

    // set rotation
  btQuaternion rot;
  rot.setEulerZYX( 0.0, angle, 0.0 ); 
  newTrans.setRotation(rot);

    // set new state
  newState->setWorldTransform(newTrans);
  enemy->rigidBody->setMotionState(newState);

}

void Graphics::setBulletMotion(int enemyID){

  // get needed objects
  Object *enemy = enemies[enemyID];
  glm::vec3 bulletDirection = enemyDirection[enemyID];
  Object **bullets = enemyBullets[enemyID];

  // set if there are unused bullets
  for( int i = 0; i < numBulletsPerEnemy; ++i ){
    if( !isBulletUsed[i] ){
      
printf("%f, %f, %f\r\n", bulletDirection.x, bulletDirection.y, bulletDirection.z);
      // get bullet position
      glm::vec3 bulletPos = glm::vec3(
                    enemy->rigidBody->getCenterOfMassPosition().getX(),
                    enemy->rigidBody->getCenterOfMassPosition().getY(),
                    enemy->rigidBody->getCenterOfMassPosition().getZ()
                  );   
printf("%f, %f, %f\r\n", bulletPos.x, bulletPos.y, bulletPos.z);
      bulletPos += normalize(bulletDirection);
printf("%f, %f, %f\r\n", bulletPos.x, bulletPos.y, bulletPos.z);

      // set bullet position

        // get current state 
        btMotionState *newState = bullets[i]->rigidBody->getMotionState();
        btTransform newTrans;
        newState->getWorldTransform(newTrans);

        // set position
        btVector3 newPos = btVector3(bulletPos.x, bulletPos.y, bulletPos.z);
        newTrans.setOrigin(newPos);

        // set new state
        newState->setWorldTransform(newTrans);
        bullets[i]->rigidBody->setMotionState(newState);

      // set bullet motion
        
        // move bullet from enemy facing direction 
        float speedReduction = 10.0;
        bullets[i]->rigidBody->setGravity(btVector3(bulletDirection.x/speedReduction,0.0,bulletDirection.z/speedReduction));


      // set flag
      isBulletUsed[i] = true;

      // end function
      return;
    }
  }
}

void Graphics::resetBullet(Object *bullet, int bulletID){

  // remove forces
  bullet->rigidBody->setGravity(btVector3(0.0,0.0,0.0));
  bullet->rigidBody->setAngularVelocity(btVector3(0.0,0.0,0.0));
  bullet->rigidBody->setLinearVelocity(btVector3(0.0,0.0,0.0));

  // translate to hidden position

    // get current state 
    btMotionState *newState = bullet->rigidBody->getMotionState();
    btTransform newTrans;
    newState->getWorldTransform(newTrans);

    // set position
    btVector3 newPos = btVector3(0.0, -15.0, 0.0);
    newTrans.setOrigin(newPos);

    // set new state
    newState->setWorldTransform(newTrans);
    bullet->rigidBody->setMotionState(newState);

  // unset flag
  isBulletUsed[bulletID] = false;  
}
