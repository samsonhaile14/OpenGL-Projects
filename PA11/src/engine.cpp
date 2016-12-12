
#include "engine.h"

Engine::Engine(string name, int width, int height, 
               float dimness, float ambDimness)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  e_dimness = dimness;
  e_ambDimness = ambDimness;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize()
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics(e_dimness,e_ambDimness);
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, movement);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }

  //mouse event
  else if (m_event.type == SDL_MOUSEMOTION){
   
   m_graphics->moveCamera( m_event.motion.xrel, m_event.motion.yrel );

  }

  //Key events
  else if (m_event.type == SDL_KEYDOWN)
  {

    // Handle key event for closing program
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }

    // Handle key event for reversing rotation
    else if (m_event.key.keysym.sym == SDLK_LEFT)
    {
      movement[0] = 1.0;
    }

    else if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
      movement[1] = 1.0;
    }

    else if (m_event.key.keysym.sym == SDLK_DOWN)
    {
      movement[0] = 0.0;
      movement[1] = 0.0;
    }

    // shaders
    else if (m_event.key.keysym.sym == SDLK_v)
    {
      m_graphics->setShaderProgram(0);
    }

    else if (m_event.key.keysym.sym == SDLK_f)
    {
      m_graphics->setShaderProgram(1);
    }

    // lighting changes
    else if (m_event.key.keysym.sym == SDLK_EQUALS)
    {
      m_graphics->addAmbientDim(-0.05f);
    }

    else if (m_event.key.keysym.sym == SDLK_MINUS)
    {
      m_graphics->addAmbientDim(0.05f);
    }

    else if (m_event.key.keysym.sym == SDLK_RIGHTBRACKET)
    {
      m_graphics->addSpecularDim(-0.05f);
    }

    else if (m_event.key.keysym.sym == SDLK_LEFTBRACKET)
    {
      m_graphics->addSpecularDim(0.05f);
    }

    // spotlight changes
    else if (m_event.key.keysym.sym == SDLK_SEMICOLON)
    {
      m_graphics->adjustSpotlightRadius(0.01f);
    }

    else if (m_event.key.keysym.sym == SDLK_QUOTE)
    {
      m_graphics->adjustSpotlightRadius(-0.01f);
    }

    else if (m_event.key.keysym.sym == SDLK_PERIOD)
    {
      m_graphics->adjustSpotlightIntensity(-0.05f);
    }


    else if (m_event.key.keysym.sym == SDLK_SLASH)
    {
      m_graphics->adjustSpotlightIntensity(0.05f);
    }

    // controls
      // up
    else if (m_event.key.keysym.sym == SDLK_w)
    {
      m_graphics->movePlayer(0, m_DT);
    }

      // down
    else if (m_event.key.keysym.sym == SDLK_s)
    {
      m_graphics->movePlayer(1, m_DT);
    }

      // left
    else if (m_event.key.keysym.sym == SDLK_a)
    {
      m_graphics->movePlayer(2, m_DT);
    }

      // right
    else if (m_event.key.keysym.sym == SDLK_d)
    {
      m_graphics->movePlayer(3, m_DT);
    }

     //space
    else if (m_event.key.keysym.sym == SDLK_SPACE)
    {
      m_graphics->movePlayer(4, m_DT);
    }    

     //stop
    else if (m_event.key.keysym.sym == SDLK_c)
    {
      m_graphics->movePlayer(5, m_DT);
    }

    // reset game
    else if (m_event.key.keysym.sym == SDLK_r)
    {
//      printf("RESET");
      m_graphics->resetGame();
    }
  } 

  else if(m_event.type == SDL_KEYUP){

    if (m_event.key.keysym.sym == SDLK_LEFT)
    {
      movement[0] = 0.0;
    }

    else if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
      movement[1] = 0.0;
    }

    // set damping to player movement
    else
    {
      m_graphics->movePlayer(6, m_DT);
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
