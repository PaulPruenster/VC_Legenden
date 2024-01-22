#version 330 core

// Ouput data
layout(location = 0) out float fragmentdepth;
out vec4 FragColor;


void main(){
	fragmentdepth = gl_FragCoord.z;
	FragColor = fragmentdepth * vec4(1.0, 1.0, 1.0, 1.0);
}