#version 330
 
          
          in vec2 TexCoord; 
          in vec4 color; 
          in vec3 light;
          
          out vec4 frag_color; 
          
          uniform sampler2D gSampler;
          
          void main(void) 
          { 
             vec4 texColor = texture2D( gSampler, TexCoord.st );
             frag_color = color * texColor;

             // check if outside of spotlight (?)
             if( dot(normalize(light), normalize(vec3(0.0,1.0,0.0))) < 0.65 )
               frag_color /= 10;
          } 

