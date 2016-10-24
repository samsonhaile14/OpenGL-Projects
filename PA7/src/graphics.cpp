#include "graphics.h"

Graphics::Graphics(std::vector< Planet > solSys )
{

setting = solSys;

}

Graphics::~Graphics()
{

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

  // Create the Objects
  int indx;
  int mIndx;

  for( indx = 0; indx < setting.size(); indx++ ){

     vector< Object > subObjs;

     Object temp( setting[indx].pAttr , "../objects/sphere.obj", 
                        "../textures/" + setting[indx].pAttr.textureFile );

     for( mIndx = 0; mIndx < setting[indx].moons.size(); mIndx++ ){
         Object subTemp( setting[indx].moons[mIndx] , "../objects/sphere.obj", 
                        "../textures/" + setting[indx].moons[mIndx].textureFile );

         subObjs.push_back( subTemp );

     }

     if( setting[indx].pAttr.hasRing ){
         Sphere ring = setting[indx].pAttr;
         ring.diameter *= 0.2;
         ring.hasRing = false;
         ring.orbitRadius = 0;
         ring.orbitSpeed = 0;
         ring.rotationPeriod = 0;

         Object subTemp( ring, "../objects/ring.obj", 
                         "../textures/" + setting[indx].pAttr.textureFile );

         subObjs.push_back( subTemp );
     }


     pObjs.push_back( temp );
     mObjs.push_back( subObjs );

  }

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

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

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
}

void Graphics::Update(unsigned int dt, float timeScale, float movement[], bool pause, bool isZoomToggled, float zoomFactor)
{

   int indx;
   int mIndx;

   for( indx = 0; indx < pObjs.size(); indx++ ){

      pObjs[indx].Update( dt, timeScale, movement, pause, isZoomToggled, zoomFactor);

      for( mIndx = 0; mIndx < mObjs[indx].size(); mIndx++ ){
         mObjs[indx][mIndx].Update(dt, timeScale, movement, pause, false, zoomFactor);
      }

   }

}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(glm::translate(glm::vec3(0.0,0.0,-20.0)) * m_camera->GetView())); 

  // Render the objects
  int indx;
  int mIndx;
  for( indx = 0; indx < pObjs.size(); indx++ ){

      glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(
                  (pObjs[indx].GetModel())
                  ));
      pObjs[indx].Render(gSampler);

      for( mIndx = 0; mIndx < mObjs[indx].size(); mIndx++ ){
         glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(
                     (pObjs[indx].GetModel() * mObjs[indx][mIndx].GetModel())
                     ));
         mObjs[indx][mIndx].Render(gSampler);         
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

// function: have camera move towards spcified position, facing origin
void Graphics::UpdateCamera(glm::vec3 pos){

  m_camera->Update(pos);
}

// function: have camera move towards a planet
void Graphics::goToPlanet(int planetIndex){

    // move camera towards planet coordinates
    glm::vec3 newPos = pObjs[planetIndex].getPosition();
    m_camera->goToPlanet(newPos);
}

// function: move camera to top down view of solar system
void Graphics::resetCamera(){
    m_camera->Update(glm::vec3(-200.0,300.0,-350.0));
}

float Graphics::getZoomFactor(int planetIndex){
    return pObjs[planetIndex].getZoomFactor();
}
