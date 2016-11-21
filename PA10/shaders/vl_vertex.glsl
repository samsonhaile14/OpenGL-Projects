#version 330
 
          
          layout (location = 0) in vec3 v_position; 
          layout (location = 1) in vec2 v_uv; 
          layout (location = 2) in vec3 v_normal;
          
          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix; 
          
          uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct; 
          uniform vec4 LightPosition; 
          uniform float Shininess; 
          uniform float SpecularDimness;

          uniform float Dimness;
          uniform float AmbientDimness;
          
          out vec2 TexCoord; 
          out vec4 color; 
          out vec3 light;
          
          void main(void) 
          { 
          
          mat4 ModelView = viewMatrix * modelMatrix; 
          
          vec3 pos = ( ModelView * vec4(v_position,1.0) ).xyz; 
          
          vec3 L = normalize(LightPosition.xyz - pos); 

          light = LightPosition.xyz;
    
          if( LightPosition.w != 0.0 ) {
         	light = LightPosition.xyz - v_position.xyz;
          }

          vec3 E = normalize(-pos); 
          vec3 H = normalize(L + E); 
          
            vec3 N = normalize( ModelView * vec4(v_normal,0.0) ).xyz; 
          
            vec4 ambient = AmbientProduct/AmbientDimness; 
            
            float Kd = max( dot(L,N),0.0 ); 
            vec4 diffuse = Kd * DiffuseProduct; 
            float Ks = pow( max(dot(N,H),0.0), Shininess ); 
            vec4 specular = Ks * SpecularProduct/SpecularDimness;
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position,1.0); 
            
            color = (ambient + diffuse + specular)/Dimness; 
            color.a = 1.0; 
            TexCoord = v_uv; 
          } 
          
