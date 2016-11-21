PA10 Run instructions

Follow the following instructions to compile and run the code:

In the 'CS680_Haile/PA9' directory, execute the following commands:

```  
mkdir build  
cd build  
cmake ..  
make  
```  
_  
_  
Instructions for executing:  
run the command ./Tutorial  
_  
The dimness of the scene, as well as the dimness of the ambient lighting can  
be specified by command line arguments. The specification is made using the  
following format:  
_  
./Tutorial |dimness| |ambient dimness|  
where |dimness| is input for the dimness of the scene and |ambient dimness| is
input for the dimness of the ambient lighting. Both must be numerical values  
_  
If the user wishes to only change ambient dimness, specify 1 for dimness, then  
specify the ambient dimness value. If the user wishes to only change dimness, 
only specify the dimness value. If the user doesn't wish to change any values,
simply execute ./Tutorial by itself.  
_  
If the user wishes to change the specular brightness of a given object, they can
change the brightness by accessing the mtl file of a given object and changing
the Ns attribute. The higher the Ns, the brighter the specular highlight the
object gives off.
_  
Controls:  
V: Switch to vertex lighting shader  
F: Switch to fragment lighting shader  
Left Arrow Key: Rotate left flipper  
Right Arrow Key: Rotate right flipper  
Down Arrow Key: Return flippers to base position  
Number keys 2-6: Change viewpoint  
Period key(.) / Forward Slash key (/): decrease/increase spotlight intensity respectively  
Semicolon key(;) / Quote key ('): decrease/increase spotlight radius respectively  
Left Bracket key([) / Right Bracket key (]): decrease/increase dimness of specular light respectively  
Minus key(-) / Equals key (=): decrease/increase dimness of ambient light respectively  

