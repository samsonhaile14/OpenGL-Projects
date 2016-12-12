#include <iostream>
#include <stdlib.h>

#include "engine.h"


int main(int argc, char **argv)
{
   std::vector< std::string > fileNames;


  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 1000, 800, 3.0,3.0);
  if(!engine->Initialize())
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
