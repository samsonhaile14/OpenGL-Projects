#version 330

          layout (location = 0) in vec3 v_position; 
          layout (location = 1) in vec2 v_uv; 
          layout (location = 2) in vec3 v_normal;



	  // output values that will be interpolatated per-fragment
	  out vec3 fN;
	  out vec3 fE;
	  out vec3 fL;
          out vec2 TexCoord;
          out float dim;
          out float AmDim;
          out float SpDim;

          uniform mat4 projectionMatrix; 
          uniform mat4 viewMatrix; 
          uniform mat4 modelMatrix; 
          uniform vec4 LightPosition; 
          uniform float SpecularDimness;

          uniform float Dimness;
          uniform float AmbientDimness;

          void main()
          {

          fN = v_normal;
          fE = v_position.xyz;
          fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
	fL = LightPosition.xyz - v_position.xyz;
    }

    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(v_position,1.0);
    TexCoord = v_uv; 
    dim = Dimness;
    AmDim = AmbientDimness;
    SpDim = SpecularDimness;
}
