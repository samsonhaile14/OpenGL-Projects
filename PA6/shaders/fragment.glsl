#version 330
 \
          \
          smooth in vec3 tCoord; \
          \
          out vec4 frag_color; \
          \
          void main(void) \
          { \
             frag_color = texture2D(gSampler, tCoord.st); \
          } \
          
