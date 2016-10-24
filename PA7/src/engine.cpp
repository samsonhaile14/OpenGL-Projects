
#include "engine.h"

Engine::Engine(string name, int width, int height, 
               std::vector< Planet > setting)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  solSys = setting;
}

Engine::Engine(string name, std::vector< Planet > setting)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
  solSys = setting;
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
  m_graphics = new Graphics(solSys);
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

      // lock onto planet if index given
    if(planetLock >= 0 && planetLock <= 9){
      zoomFactor = m_graphics->getZoomFactor(planetLock);
      m_graphics->goToPlanet(planetLock);
    }
      
    else{
      m_graphics->resetCamera();
      isZoomToggled = false;
    }

    m_graphics->Update(m_DT, timeScale, movement, pause, isZoomToggled, zoomFactor);
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

  //Key events
  else if (m_event.type == SDL_KEYDOWN)
  {

    // Handle key event for closing program
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }

    // Handle key event for reversing rotation
    else if (m_event.key.keysym.sym == SDLK_r)
    {
      movement[0] *= -1.0;
    }

    // Handle key event for reversing orbit
    else if (m_event.key.keysym.sym == SDLK_o ||
             (m_event.key.keysym.sym == SDLK_LEFT && movement[1] > 0) ||
             (m_event.key.keysym.sym == SDLK_RIGHT && movement[1] < 0))
    {
      movement[1] *= -1.0;
    }

    // Handle key event for stopping/resuming rotation
    else if (m_event.key.keysym.sym == SDLK_t)
    {
      if((movement[0] == 1) || (movement[0] == -1))
      {
        movement[0] += movement[0];
      }

      else
      {
        movement[0] -= movement[0]/2.0f;
      } 
    }

    // Handle key event for stopping/resuming orbit
    else if (m_event.key.keysym.sym == SDLK_p)
    {
      if( (movement[1] == 1.0f) || (movement[1] == -1.0f))
      {
        movement[1] += movement[1];
      }

      else
      {
        movement[1] -= movement[1]/2.0f;
      }
    }

    // controls to view planets/solar system
    else if (m_event.key.keysym.sym == SDLK_1)
    {
      planetLock = 0;
    }

    else if (m_event.key.keysym.sym == SDLK_2)
    {
      planetLock = 1;
    }

    else if (m_event.key.keysym.sym == SDLK_3)
    {
      planetLock = 2;
    }

    else if (m_event.key.keysym.sym == SDLK_4)
    {
      planetLock = 3;
    }


    else if (m_event.key.keysym.sym == SDLK_5)
    {
      planetLock = 4;
    }


    else if (m_event.key.keysym.sym == SDLK_6)
    {
      planetLock = 5;
    }


    else if (m_event.key.keysym.sym == SDLK_7)
    {
      planetLock = 6;
    }


    else if (m_event.key.keysym.sym == SDLK_8)
    {
      planetLock = 7;
    }


    else if (m_event.key.keysym.sym == SDLK_9)
    {
      planetLock = 8;
    }

    else if (m_event.key.keysym.sym == SDLK_q)
    {
      planetLock = 9;
    }

    else if (m_event.key.keysym.sym == SDLK_0)
    {
      planetLock = -1;
    }

    else if (m_event.key.keysym.sym == SDLK_z)
    {
      isZoomToggled ^= 1;
    }

    //controls for speeding up/slowing down time
    else if (m_event.key.keysym.sym == SDLK_EQUALS)
    {
      timeScale *= 2.0;

      printf( "Speed now at %f scale\n", timeScale );
    }

    else if (m_event.key.keysym.sym == SDLK_MINUS )
    {

      if( timeScale > 1.0 ){
         timeScale /= 2.0;

         printf( "Speed now at %f scale\n", timeScale );
      }


    }

  }

  // Handle mouse event to pause everything
  else if(m_event.type == SDL_MOUSEBUTTONDOWN)
  {
  pause = !pause;
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

