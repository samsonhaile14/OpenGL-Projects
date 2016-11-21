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
g_spotlightIntensity = 1.0;
score = 0;
lives = 3;
isGameOver = false;

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

    if( lFlipper != NULL ){
     delete lFlipper;
     lFlipper = NULL;
    }

    if( rFlipper != NULL ){
     delete rFlipper;
     rFlipper = NULL;
    }

    if( bumperOne != NULL ){
     delete bumperOne;
     bumperOne = NULL;
    }

    if( ball != NULL ){
     delete ball;
     ball = NULL;
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

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create collider environment
  broadphase = new btDbvtBroadphase();
  collisionConfiguration = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfiguration);
  solver = new btSequentialImpulseConstraintSolver();
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, 
                                              collisionConfiguration);

  // Create the objects
   board = new Object(0.0,4.0,6.5, //position
                      0.0,(2.0 * 3.141592) / 4.0,0.0 , //rotation
                     0.0, 0,"../objects/cabinet.obj");   //mass,meshtype,objfile

   lFlipper = new Object(5.5,0.0,-6.5, //position
                      0.0,-(2.0 * 3.141592) / 12.0,0.0 , //rotation
                     5.0, 0,"../objects/flipper.obj");   //mass,meshtype,objfile

   rFlipper = new Object(-5.5,0.0,-6.5, //position
                      0.0,(2.0 * 3.141592) / 2.0 + (2.0 * 3.141592) / 12.0,0.0 , //rotation
                     5.0, 0,"../objects/flipper.obj");   //mass,meshtype,objfile

   bumperOne = new Object( 0.0,0.0,12.5,
                          0.0,0.0,0.0,
                          1.0,0,"../objects/bumper.obj" );

   bumperTwo = new Object( -5.0,0.0,11.0,
                          0.0,0.0,0.0,
                          1.0,0,"../objects/bumper.obj" );

   bumperThree = new Object( 5.0,0.0,11.0,
                          0.0,0.0,0.0,
                          1.0,0,"../objects/bumper.obj" );


   ball = new Object(-11.0f,0.6f,-2.4f,
                     0.0,0.0,0.0,
                     1.0, 1,"../objects/sphere.obj");

  
   plunger = new Object(-11.5f,0.0f,-6.0f,
                     0.0,0.0,0.0,
                     1.0, 1,"../objects/plunger.obj");

  SLOPE = 0.1;
  dynamicsWorld->setGravity(btVector3(0.0,-9.81,-1.0 * SLOPE));

  //Set kinematic/static rigidbodies and add rigidbodies to dynamicWorld

  int ballCollide = COL_WALL;

  dynamicsWorld->addRigidBody(bumperOne->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(bumperTwo->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(bumperThree->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(board->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(ball->rigidBody,COL_BALL, ballCollide);
  dynamicsWorld->addRigidBody(lFlipper->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(rFlipper->rigidBody,COL_WALL,COL_BALL);
  dynamicsWorld->addRigidBody(plunger->rigidBody,COL_WALL,COL_BALL);

  board->rigidBody->setCollisionFlags(board->rigidBody->getCollisionFlags() | 
                                      btCollisionObject::CF_KINEMATIC_OBJECT);
  board->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  bumperOne->rigidBody->setCollisionFlags(bumperOne->rigidBody->getCollisionFlags() | 
                                      btCollisionObject::CF_KINEMATIC_OBJECT);
  bumperOne->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  bumperTwo->rigidBody->setCollisionFlags(bumperTwo->rigidBody->getCollisionFlags() | 
                                      btCollisionObject::CF_KINEMATIC_OBJECT);
  bumperTwo->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  bumperThree->rigidBody->setCollisionFlags(bumperThree->rigidBody->getCollisionFlags() | 
                                      btCollisionObject::CF_KINEMATIC_OBJECT);
  bumperThree->rigidBody->setActivationState(DISABLE_DEACTIVATION);


  ball->rigidBody->setActivationState(DISABLE_DEACTIVATION);

  lFlipper->rigidBody->setActivationState(DISABLE_DEACTIVATION);
  lFlipper->rigidBody->setLinearFactor( btVector3( 0.0,0.0,0.0 ));

  rFlipper->rigidBody->setActivationState(DISABLE_DEACTIVATION);
  rFlipper->rigidBody->setLinearFactor( btVector3( 0.0,0.0,0.0 ));

  plunger->rigidBody->setActivationState(DISABLE_DEACTIVATION);
  plunger->rigidBody->setLinearFactor( btVector3( 0.0,0.0,0.0 ));
  plunger->rigidBody->setAngularFactor ( btVector3( 0.0,0.0,0.0 ));


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
   lightPos = glm::vec4( 100.0,-30.0,3.0,1.0 );

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, float movement[])
{

  dynamicsWorld->stepSimulation(dt,5);

  btTransform trans;
  btQuaternion rot;

  float flipperSpeed = 5.0;
  btScalar yaw, pitch, roll;

  // left flipper data
  lFlipper->rigidBody->getMotionState()->getWorldTransform(trans);
  btMatrix3x3 basis = trans.getBasis();
  basis.getEulerZYX(yaw,pitch,roll);
  
  // left flipper key pressed
  if(movement[0] > 0){   

  if( pitch < 0.4)
    lFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,flipperSpeed,0.0 ));

  else if( pitch > 0.4 )
    lFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,0.0,0.0 ));
  }  

  // left flipper key not pressed
  else{

    if( pitch > 0.4)
      lFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,-flipperSpeed,0.0 ));

    else if( pitch < -0.12 )
      lFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,0.0,0.0 ));
  }

  // right flipper data
  rFlipper->rigidBody->getMotionState()->getWorldTransform(trans);
  basis = trans.getBasis();
  basis.getEulerZYX(yaw,pitch,roll);

  // right flipper key pressed
  if(movement[1] > 0){

    if( pitch < 0.4)
      rFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,-flipperSpeed,0.0 ));

    else if( pitch > 0.12 )
      rFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,0.0,0.0 ));
  }


  // not pressed
  else{
 
    if( pitch > 0.4)
      rFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,flipperSpeed,0.0 ));

    else if( pitch < -0.12 )
      rFlipper->rigidBody->setAngularVelocity( btVector3( 0.0,0.0,0.0 ));
  }


  
  // plunger data
  plunger->rigidBody->getMotionState()->getWorldTransform(trans);
  btVector3 depr = trans.getOrigin();

  //basis.getEulerZYX(yaw,pitch,roll);

  //spacebar is depressed 
  if(movement[2] == 1){
      if ( depr.getZ() > -12.0 ) {
          plunger->rigidBody->setLinearVelocity (btVector3 (0.0, 0.0, -0.1));
          movement[3] += 0.01;
      }
      else
          plunger->rigidBody->setLinearVelocity (btVector3 (0.0, 0.0, 0.0));
  }
  
  //spacebar is released
  else if (movement[2] == -1){
      plunger->rigidBody->setLinearVelocity (btVector3 (0.0, 0.0, movement[3]));
      movement[2] = 0;
  }

  // in motion or in standstill
  else{
      if (depr.getZ() > -4) {
          plunger->rigidBody->setLinearVelocity (btVector3 (0.0, 0.0, 0.0));
          movement[3] = 0.0;
      }
      
  }

  bumperOne->Update( dt,movement,false);
  bumperTwo->Update( dt,movement,false);
  bumperThree->Update( dt,movement,false);

  board->Update( dt, movement, false);

  ball->Update(dt,movement,false);
  plunger->Update(dt,movement,false);
  lFlipper->Update(dt,movement,false);
  rFlipper->Update(dt,movement,false);

  // check if life lost
  glm::mat4 ballMatrix = ball->GetModel();
  glm::vec4 ballPosition = glm::vec4(ballMatrix[3]);

  if( ballPosition.z < -10.0 && lives <= 0 ){
    --lives;
    if( lives <= 0 ){
      printf("GAME OVER.\r\nFINAL SCORE: %d\r\n", score);
    }
    else{
      printf("Lives left: %d\r\n", lives);
      ballMatrix[3].x = -12.0;
      ballMatrix[3].y = 0.6;
      ballMatrix[3].z = -2.4;
      ball->setModel(ballMatrix);
    }
  }

  // update lighting
  lightPos.x = -ballPosition.z+6.5;
  lightPos.y = ballPosition.y+3;
  lightPos.z = ballPosition.x;
  lightPos.a = 1.0;


  // update score
  int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
  for( int i = 0; i < numManifolds; ++i ){
    btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);


    const btCollisionObject *obA = contactManifold->getBody0();
    const btTransform transA = obA->getWorldTransform();
    const btVector3 originA = transA.getOrigin();

    const btCollisionObject *obB = contactManifold->getBody1();
    const btTransform transB = obB->getWorldTransform();
    const btVector3 originB = transB.getOrigin();

    // check if any collision for bumper one detected
    if( originA.getX() == bumperOne->globalPos.x 
        &&  originA.getY() == bumperOne->globalPos.y 
        &&  originA.getZ() == bumperOne->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 

    if( originB.getX() == bumperOne->globalPos.x 
        &&  originB.getY() == bumperOne->globalPos.y 
        &&  originB.getZ() == bumperOne->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 

    // check if any collision for bumper two detected
    if( originA.getX() == bumperTwo->globalPos.x 
        &&  originA.getY() == bumperTwo->globalPos.y 
        &&  originA.getZ() == bumperTwo->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 

    if( originB.getX() == bumperTwo->globalPos.x 
        &&  originB.getY() == bumperTwo->globalPos.y 
        &&  originB.getZ() == bumperTwo->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 


    // check if any collision for bumper three detected
    if( originA.getX() == bumperThree->globalPos.x 
        &&  originA.getY() == bumperThree->globalPos.y 
        &&  originA.getZ() == bumperThree->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 

    if( originB.getX() == bumperThree->globalPos.x 
        &&  originB.getY() == bumperThree->globalPos.y 
        &&  originB.getZ() == bumperThree->globalPos.z )
    {
      score += 10;
      printf("Score: %d\r\n", score);
    } 
   }
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
  glUniform1f( l_dimness, g_dimness );
  glUniform1f( l_ambDimness, g_ambDimness );
  glUniform1f( l_specDimness, g_specDimness );
  glUniform1f( l_cutoffAngle, g_cutoffAngle );
  glUniform1f( l_spotlightIntensity, g_spotlightIntensity );

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView())); 

  // Render the objects  
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumperOne->GetModel()));
  bumperOne->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);


  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumperTwo->GetModel()));
  bumperTwo->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumperThree->GetModel()));
  bumperThree->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
  board->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
  ball->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(lFlipper->GetModel()));
  lFlipper->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rFlipper->GetModel()));
  rFlipper->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(plunger->GetModel()));
  plunger->Render(gSampler,l_amb, l_dif, l_spec, l_shininess);

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

void Graphics::moveCamera(int id){
  m_camera->setView(id);
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
  glm::mat4 ballMatrix = ball->GetModel();

  ballMatrix[3].x = -12.0;
  ballMatrix[3].y = 0.6;
  ballMatrix[3].z = -2.4;

  ball->setModel(ballMatrix);

  lives = 3;
  score = 0;
}

