#include <iostream>
#include <stdlib.h>

#include "engine.h"


int main(int argc, char **argv)
{
   std::vector< std::string > fileNames;
   float dimness;

  //take object files
  if( argc > 1 ){
  
    dimness = atof(argv[1]);

    if( dimness == 0 ){
      printf( "Error, dimness has invalid value\n" );
      return 1;
    }

  }

  else{

   dimness = 1.0;

  }



  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600, dimness);
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
