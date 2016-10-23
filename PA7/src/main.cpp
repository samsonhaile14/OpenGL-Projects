#include <iostream>
#include <fstream>

#include "engine.h"

void normData( std::vector<Planet> &data);
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

void normData( std::vector<Planet> &data){
    float sum_dia = 0;
    float mSum_dia = 0;
    float sum_rad = 0;
    float mSum_rad = 0;
    float sum_speed = 0;
    float mSum_speed = 0;
    float sum_per = 0;
    float mSum_per = 0;

    for ( int i = 0; i < data.size(); i++){
        sum_dia += data[i].pAttr.diameter;
        sum_rad += data[i].pAttr.orbitRadius;
        sum_speed += data[i].pAttr.orbitSpeed;
        sum_per += data[i].pAttr.rotationPeriod;

        mSum_dia = 0;
        mSum_rad = 0;
        mSum_speed = 0;
        mSum_per = 0;

       for ( int j = 0; j < data[i].moons.size(); j++){
           mSum_dia += data[i].moons[j].diameter;
           mSum_rad += data[i].moons[j].orbitRadius;
           mSum_speed += data[i].moons[j].orbitSpeed;
           mSum_per += data[i].moons[j].rotationPeriod;

       }
cout << mSum_dia << ' ' << mSum_rad << ' ' << mSum_speed << ' ' << mSum_per << endl;
       for ( int j = 0; j < data[i].moons.size(); j++){

           if( mSum_dia != 0 )
              data[i].moons[j].diameter /= sum_dia * 10.0;

           if( mSum_rad != 0 )            
            data[i].moons[j].orbitRadius /= sum_rad * 10.0;

           if( mSum_speed != 0 )
              data[i].moons[j].orbitSpeed /= sum_speed * 10.0;

           if( mSum_per != 0 )
              data[i].moons[j].rotationPeriod /= sum_per * 10.0;

       } 

    }

    for ( int i = 0; i < data.size(); i++){
           if( sum_dia != 0 )
              data[i].pAttr.diameter /= sum_dia;

           if( sum_rad != 0 )            
            data[i].pAttr.orbitRadius /= sum_rad;

           if( sum_speed != 0 )
              data[i].pAttr.orbitSpeed /= sum_speed;

           if( sum_per != 0 )
              data[i].pAttr.rotationPeriod /= sum_per;    }

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
   
   normData(setting);
   
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
