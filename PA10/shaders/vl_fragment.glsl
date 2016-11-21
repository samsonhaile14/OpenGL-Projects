#version 330
 
          
          in vec2 TexCoord; 
          in vec4 color; 
          in vec3 light;
          in vec3 lightB;
          in vec3 lightC;
          
          out vec4 frag_color; 
          
          uniform float CutoffAngle;
          uniform sampler2D gSampler;
          uniform float SpotlightIntensity;
          
          void main(void) 
          { 
             vec4 texColor = texture2D( gSampler, TexCoord.st );
             frag_color = color * texColor;

             // check if outside of spotlight (?)
             if( dot(normalize(light), normalize(vec3(0.0,1.0,0.0))) < CutoffAngle && 
                 dot(normalize(lightB), normalize(vec3(0.0,1.0,0.0))) < CutoffAngle &&
                 dot(normalize(lightC), normalize(vec3(0.0,1.0,0.0))) < CutoffAngle )
               frag_color /= 10;

             else
               frag_color *= SpotlightIntensity;
          } 

