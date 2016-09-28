#include "object.h"

Object::Object(float oRadius, float oSpeed)
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }


init(oRadius, oSpeed);
}

Object::Object(float oRadius, float oSpeed, std::string objPath )
{

//variables and initialization
int indx;
float x,y,z;
unsigned int val;
std::vector<glm::vec3> colorList(1, {0,0,0});
std::vector<std::string> colorName(1, "None");

//read obj file
if( !loadObj( objPath, colorList, colorName  ) ){
   std::cout << "fail, passing empty object" << std::endl;
   return;
}

init(oRadius, oSpeed);

}

Object::Object(float oRadius, float oSpeed, std::string objPath, 
               std::string mtlPath )
{

//variables and initialization
int indx;
float x,y,z;
std::string start, strVal;

//open mtl file
std::ifstream fAccess(mtlPath, std::fstream::in);

//If file does not exist or access fails, leave content empty
if( fAccess.fail() ){
std::cout << "fail, passing empty object" << std::endl;
      return;
}


//read material attributes
   std::vector< glm::vec3 > colorList;
   std::vector< std::string > colorName;
   float rCol, gCol, bCol;
   int numMtls;

   //ignore up to material count
   for( indx = 0; indx < 2;indx++){
      fAccess.ignore(999, ':');
   }

   //get number of materials
   fAccess >> numMtls;

   for( indx = 0; indx < numMtls; indx++ ){
      fAccess >> start >> strVal;
      colorName.push_back(strVal);

      //ignore up to Kd line
      fAccess.ignore(999, 'K');
      fAccess.ignore(999, 'K');
      fAccess.ignore(999, 'd');

      //retrieve rgb values
      fAccess >> rCol >> gCol >> bCol;

      colorList.push_back( {rCol,gCol,bCol} );

      //ignore till next material or end of file
      fAccess.ignore(999, 'n');
   }

fAccess.close();

//read obj file
if( !loadObj( objPath, colorList, colorName  ) ){
   std::cout << "fail, passing empty object" << std::endl;
   return;
}

init(oRadius, oSpeed);

}


void Object::init( float oRadius, float oSpeed )
{


  rotAngle = 0.0f;
  orbitAngle = 0.0f;
  orbitRadius = oRadius;
  orbitSpeed = oSpeed;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


}

int Object::loadObj( std::string fName, std::vector< glm::vec3 > colorList,
                      std::vector< std::string > colorNames )
{

//variables
std::string start, mtlName;
unsigned int val;
float xPos,yPos,zPos;
long long int indx, mtlIndx;

//open obj file
   std::ifstream fAccess(fName, std::fstream::in);

   //If file does not exist or access fails, return 0 for failure
   if( fAccess.fail() ){
      return 0;
   }


//read object attributes
   //skip to vertex input
      fAccess >> start;
      while(start != "v"){
         fAccess >> start;
      }

   //read vertices
      while(start == "v" || start == "vn"){
         fAccess >> xPos >> yPos >> zPos;
         Vertices.push_back({{xPos, yPos, zPos}, {0.0f, 0.0f, 0.0f}});
         fAccess >> start;
      }

   //collect faces of each material type
      std::vector< bool > colorSet(Vertices.size(),false); //checks if color of vertex is assigned

      //move file pointer to material name token
         while(start != "usemtl" && fAccess.good()){
            fAccess >> start;
         }

      while(fAccess.good() && start == "usemtl"){
         //vector for vertex-color tracking
            std::vector< long long int > assignedIndx(colorSet.size(), -1);

         //determine material used for faces
            fAccess >> mtlName;

            mtlIndx = 0;
            while( colorNames[mtlIndx] != mtlName ){
               mtlIndx++;

               if( mtlIndx >= colorList.size() ){
                  mtlIndx = 0;
                  mtlName = colorNames[mtlIndx];
               } 
            }


         //skip to face input
            fAccess >> start;
            while(start != "f"){
               fAccess >> start;
            }

         //read faces of selected material
            while(fAccess.good() && start == "f"){

               for( indx = 0; indx < 3; indx++ ){
                  fAccess >> val; 
                  val-=1;

                  if( !colorSet[val] ){
                     colorSet[val] = true;
                     Vertices[val].color = colorList[mtlIndx];
                     assignedIndx[val] = val;
                  }

                  else if(assignedIndx[val] == -1){
                     assignedIndx[val] = Vertices.size();
                     Vertices.push_back( {Vertices[val].vertex, colorList[mtlIndx]} );
                     val = assignedIndx[val];
                  }

                  else{
                     val = assignedIndx[val];
                  }
   std::cout << val << ' ';
                  Indices.push_back(val);

                  //pickup extraneous characters after index
                  if( fAccess.peek() != ' ' ){
                     fAccess >> start;
                  }
               }

            //read start of next line
            fAccess >> start;
   std::cout << start << std::endl;
            }

      }

   fAccess.close();

   return 1;

}


Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, float movement[], bool pause)
{
  

}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

