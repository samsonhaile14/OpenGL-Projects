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
           std::vector< std::string > fileNames );
    Engine(string name);
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
    std::vector< std::string > gFiles;
    SDL_Event m_event;

    Graphics *m_graphics;

    float movement[2] = {0.0f,0.0f}; 
    bool pause = false; 


    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;
};

#endif // ENGINE_H
