#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{
   std::vector< std::string > fileNames;

  //take object files
  if( argc > 1 ){
  
     std::string fName;

     fName = argv[1];
     fileNames.push_back(fName);

     if( argc > 2 ){
        fName = argv[2];
        fileNames.push_back(fName);
     } 

  }

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600, fileNames);
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
