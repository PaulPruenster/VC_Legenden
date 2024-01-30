#version 330 core

in vec4 tColor;
in vec2 tUV;
in vec4 ShadowCoord;

out vec4 FragColor;

uniform sampler2D textureMap;
uniform sampler2DShadow shadowMap;

uniform bool showShadow;
uniform bool addBias;
uniform bool showAntiAliasing;

float random(vec4 seed4) {
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

void main(void)
{
    //FragColor = vec4(texture(textureMap, tUV).rgb , 1.0);

    	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	
	// Material properties
	vec3 MaterialDiffuseColor = texture( textureMap, tUV ).rgb;

	vec2 poissonDisk[4] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 )
	);
	float bias = 0.000;
	if (addBias){
		bias = 0.005;
	}
	
	float visibility = 1.0;
	if (showShadow){
		visibility -= 0.8*(1.0-texture( shadowMap, vec3(ShadowCoord.xy,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
		if (showAntiAliasing){
			visibility = 1.0;
			for (int i=0;i<4;i++){
		        visibility -= 0.18*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + poissonDisk[i]/1500.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
		    }
		}

	}

	FragColor = vec4(visibility * MaterialDiffuseColor * LightColor, 1.0);
}