#include <iostream>
#include <fstream>

#include "engine.h"

void configRead( std::vector<Planet> &setting, fstream &fin );
std::string readSphere( Sphere &obj, fstream &fin );


int main(int argc, char **argv)
{
   std::vector< std::string > fileNames;
   std::vector<Planet> setting;

   //read config file (is constant)
      fstream fin;

      fin.open("../data.config");

      if(!fin.good()){
         printf( "configuration file not found.\n" );
         return 1;
      }

      else{
         configRead(setting, fin);
      }

      fin.close();

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600, setting);
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

void configRead( std::vector<Planet> &setting, fstream &fin ){
   
   std::string str = "";
   float val;
   
   int indx;

   while( str != "StartPlanetsData" && fin.good() ){
      fin >> str;
   }


   fin >> str; //read type designation
   while( str != "EndPlanetsData" && fin.good() ){
     
      Planet obj;
      str = readSphere( obj.pAttr, fin );
      setting.push_back( obj );

   }

   str = "";

   while( str != "StartMoonsData" && fin.good() ){
      fin >> str;
   }

   fin >> str; //read type designation
   while( str != "EndMoonsData" && fin.good() ){

      Sphere obj;
      fin >> str; //read name of planet sattelite orbits

      indx = 0;
      while( setting[ indx ].pAttr.name != str && indx < setting.size() ){
         indx++;
         }

      str = readSphere( obj, fin );
      setting[indx].moons.push_back( obj );      

   }

   
}

std::string readSphere( Sphere &obj, fstream &fin ){

   std::string str;
   float val;

   fin >> str; //read planet name
   obj.name = str;
      
   for( int indx = 0; indx < 4; indx++ ){

      fin >> val; //read numerical value

      if(indx == 0){
         obj.diameter = val;
      }

      else if(indx == 1){
         obj.orbitRadius = val;
      }

      else if(indx == 2){
         obj.orbitSpeed = val;
      }

      else if(indx == 3){
         obj.rotationPeriod = val;
      }

   }

   fin >> str;
   obj.textureFile = str;

   fin >> str; //read next type designation
   return str;

}
