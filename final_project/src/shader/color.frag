#version 330 core

in vec4 tColor;
in vec2 tUV;
in vec4 ShadowCoord;

out vec4 FragColor;

uniform sampler2D textureMap;
uniform sampler2DShadow shadowMap;


void main(void)
{
    //FragColor = vec4(texture(textureMap, tUV).rgb , 1.0);

    	// Light emission properties
	vec3 LightColor = vec3(1,1,1);
	
	// Material properties
	vec3 MaterialDiffuseColor = texture( textureMap, tUV ).rgb;

	float bias = 0.005;
	float visibility = 1.0;
	if ( texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) )  <  ShadowCoord.z-bias){
		visibility = 0.3;
	}
	FragColor = vec4(visibility * MaterialDiffuseColor * LightColor, 1.0);
}