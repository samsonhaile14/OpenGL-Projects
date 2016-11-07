#version 330
 \
          \
          in vec2 TexCoord; \
          \
          out vec4 frag_color; \
          \
          uniform sampler2D gSampler;
          \
          void main(void) \
          { \
             frag_color = vec4(vec3(1.0,1.0,1.0),1.0);\
          } \
          
