#include <iostream>
#include <stdlib.h>

#include "engine.h"


int main(int argc, char **argv)
{
   std::vector< std::string > fileNames;
   float dimness, ambDimness;

  //take object files
  if( argc > 1 ){
  
    dimness = atof(argv[1]);

    if( dimness == 0 ){
      printf( "Error, dimness has invalid value\n" );
      return 1;
    }

   if( argc > 2 ){
      ambDimness = atof(argv[2]);
   }

   else{
      ambDimness = 1.0;
   }

  }

  else{

   dimness = 1.0;
   ambDimness = 1.0;

  }



  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600, 3.0,3.0);
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
