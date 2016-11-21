#version 330

	// per-fragment interpolated values from the vertex shader
	in vec3 fN;
	in vec3 fL;
	in vec3 fE;
        in vec2 TexCoord;
        in float dim;
        in float AmDim;

	uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
	uniform mat4 ModelView;
	uniform vec4 LightPosition;
	uniform float Shininess;
        uniform sampler2D gSampler;

	out vec4 frag_color;

	void main() 
	{ 
    	// Normalize the input lighting vectors
    
    	vec3 N = normalize(fN);
   	vec3 E = normalize(fE);
  	vec3 L = normalize(fL);
	
   	vec3 H = normalize( L + E );   
   	vec4 ambient = AmbientProduct/AmDim;

	float Kd = max(dot(L, N), 0.0);
    	vec4 diffuse = Kd*DiffuseProduct;
    
    	float Ks = pow(max(dot(N, H), 0.0), Shininess);
    	vec4 specular = Ks*SpecularProduct;

    	// discard the specular highlight if the light's behind the vertex
    	if( dot(L, N) < 0.0 ) 
		specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    	frag_color = (ambient + diffuse + specular)/dim;
    	frag_color.a = 1.0;
        vec4 texColor = texture2D( gSampler, TexCoord.st );
        frag_color = frag_color * texColor;

        // check if outside spotlight range
        if( dot(L, normalize(vec3(0.0,1.0,0.0))) < 0.65 )
          frag_color /= 10;
	} 




