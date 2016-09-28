Follow the following instructions to compile and run the code:

1. In the 'CS680_Haile/PA4' directory, execute the command mkdir build
2. Execute the command cd build
3. Execute the command cmake ..
4. Execute the command make    



Instructions for executing
The program accepts command line arguments that specify an obj and/or mtl file.  
The format of the execution of the program follows:  
./Tutorial <obj file name> <mtl file name>, where <obj file name> and  
<mtl file name> each specify a file of the respective type. For example,  
./Tutorial box.obj and box.mtl will display a box object in accordance with  
the specified obj and mtl file.    

If the user wants to use his/her own objects, they can move their objects into  
the objects directory of PA4 and specify those objects when running the program.    

The program is able to run with only a obj file  
(e.g. command: "./Tutorial box.obj" will work)  
