#version 330 core
// This is a sample fragment shader.

in float sampleExtraOutput;
in vec3 normalm;
in vec3 normalw;
in vec3 posOutput;

uniform vec3 color;
uniform bool displayNormals;
uniform vec3 ka; 
uniform vec3 kd;
uniform vec3 ks;
uniform float p;
uniform vec3 lightPos; //= vec3(0.0,0.0,20.0);
uniform vec3 lightCol; //= vec3(1.0,1.0,1.0);
uniform vec3 lightAtten; //= vec3(0.1,0.1,0.1);
uniform vec3 eyePos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    // fragColor = vec4(color, sampleExtraOutput);
	if (displayNormals){
		fragColor = vec4(normalm, 1);
	}
	else {
		//fragColor = vec4(posOutput, 1);
		vec3 L = normalize(lightPos-posOutput);
		vec3 cl = lightCol/(lightAtten[1]*distance(lightPos,posOutput));
		vec3 ambient = ka;
		vec3 diffuse = kd*clamp(dot(L,normalw),0,1);
		vec3 reflVec= 2*clamp(dot(L,normalw),0,1)*normalw-L;
		vec3 viewVec= normalize(eyePos-posOutput);
		float spec = pow(clamp(dot(viewVec, reflVec), 0,1 ), p);
		vec3 specular = ks*spec;
		fragColor = vec4(cl*(ambient+diffuse+specular),1);
	}

}