#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height, 
           std::vector< Planet > setting );
    Engine(string name, std::vector< Planet > setting );
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
  
  private:
    // Window related variables
    Window *m_window;    
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

    Graphics *m_graphics;

    std::vector<Planet> solSys;

    float movement[2] = {1.0f,1.0f}; //index 0 = rotation, index 1 = orbit
    bool pause = false; 


    unsigned int m_DT;
    float timeScale = 1;
    long long m_currentTimeMillis;
    bool m_running;

    int planetLock = -1;
    bool isZoomToggled = false;
    float zoomFactor = 0.0f;
};

#endif // ENGINE_H
