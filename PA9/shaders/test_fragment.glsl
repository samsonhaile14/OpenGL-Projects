#version 330
 \
          \
          in vec2 TexCoord; \
          in vec4 color; \
          \
          out vec4 frag_color; \
          \
          uniform sampler2D gSampler;
          \
          void main(void) \
          { \
             vec4 texColor = texture2D( gSampler, TexCoord.st );
             frag_color = color;\
          } \

