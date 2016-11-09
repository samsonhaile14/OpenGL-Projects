#version 330
 \
          \
          layout (location = 0) in vec3 v_position; \
          layout (location = 1) in vec2 v_uv; \
          layout (location = 2) in vec3 v_normal;\
          \
          uniform mat4 projectionMatrix; \
          uniform mat4 viewMatrix; \
          uniform mat4 modelMatrix; \
          \
          uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct; \
          uniform vec4 LightPosition; \
          uniform float Shininess; \
          \
          out vec2 TexCoord; \
          out vec4 color; \
          \
          void main(void) \
          { \
          \
          mat4 ModelView = viewMatrix * modelMatrix; \
          \
          vec3 pos = ( ModelView * vec4(v_position,1.0) ).xyz; \
          \
          vec3 L = normalize(LightPosition.xyz - pos); \
          vec3 E = normalize(-pos); \
          vec3 H = normalize(L + E); \
          \
            vec3 N = normalize( ModelView * vec4(v_normal,0.0) ).xyz; \
          \
            vec4 ambient = AmbientProduct; \
            \
            float Kd = max( dot(L,N),0.0 ); \
            vec4 diffuse = Kd * DiffuseProduct; \
            float Ks = pow( max(dot(N,H),0.0), Shininess ); \
            vec4 specular = Ks * SpecularProduct; \
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position,1.0); \
            \
            color = (ambient + diffuse + specular); \
            color.a = 1.0; \
            TexCoord = v_uv; \
          } \
          
