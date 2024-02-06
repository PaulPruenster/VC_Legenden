#version 330 core


in vec4 tColor;
in vec3 tNormal;
in vec2 tUV;
in vec4 ShadowCoord;

out vec4 FragColor;

uniform vec3 uLightDir;
uniform sampler2D textureMap;
uniform sampler2DShadow shadowMap;

uniform bool showShadow;
uniform bool addBias;
uniform bool addDynamicBias;
uniform bool showAntiAliasing;
uniform bool showStratified;

float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main(void)
{
	vec3 LightColor = vec3(1,1,1);
	
	vec3 MaterialDiffuseColor = texture( textureMap, tUV ).rgb;
	
	// hard coded poissonDisc samples
	vec2 poissonDisc[4] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 )
	);

	float bias = 0.000;
	if (addBias){
		if (addDynamicBias){
			float theta = acos(clamp(dot(normalize(tNormal), normalize(uLightDir)), -1, 1));
			bias = 0.005*sin(theta);
		} else {
			bias = 0.005;
		}
	}
	
	float visibility = 1.0;
	if (showShadow){
		if (showAntiAliasing){
		int index = 0;
			for (int i=0;i<4;i++){
				index = i;

				// set index to random value between 0 and 3
				if (showStratified){
					index = int(4.0*random(ShadowCoord.xyy, i))%4;
				}
				visibility -= 0.18*(1.0-texture( shadowMap, vec3(ShadowCoord.xy + poissonDisc[index]/3000.0,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
		    }
		}
		else {
			visibility -= 0.18 * 4 *(1.0-texture( shadowMap, vec3(ShadowCoord.xy,  (ShadowCoord.z-bias)/ShadowCoord.w) ));
		}
	}

	FragColor = vec4(visibility * MaterialDiffuseColor * LightColor, 1.0);
}