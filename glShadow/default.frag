#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

in SHADEROUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} shaderin;


layout(std140) uniform pseudos{
	vec2 pseudoPairs[600];
};


// Gets the Texture Units from the main function
uniform vec2 shadowMapSize;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D depthMap;
uniform sampler2D fluxMap;
uniform sampler2D normalMap;
uniform sampler2D coordMap;


// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;
uniform float lightSize;
uniform bool isFloor;
uniform bool shadowFull;

uniform int NUM_SAMPLES;


vec3 calculateIndirectLighting(){
	vec4 fragPos = shaderin.FragPosLightSpace;
	vec3 projCoords = fragPos.xyz/ fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;

	vec3 indirectContribution = vec3(0.0f);
	for(int i = 0 ; i < NUM_SAMPLES; i++){
		vec2 offset = pseudoPairs[i]/shadowMapSize;
		vec2 pixelLight = projCoords.xy + offset;
		vec3 pixelFlux = texture(fluxMap, pixelLight).xyz;
		vec3 pixelCoord = texture(coordMap, pixelLight).xyz;
		vec3 pixelNormal = texture(normalMap, pixelLight).xyz;

		vec3 pixelContrib = pixelFlux * max(0.0f, dot(pixelNormal, crntPos - pixelCoord)) * max(0.0f, dot(Normal, pixelCoord - crntPos))/(pow(length(crntPos - pixelCoord), 4));
		indirectContribution += pixelContrib;
		
	}
	indirectContribution /= 100;
	return clamp(indirectContribution, 0.0f, 1.0f);
}

float calculatePercentage(int windowSize, float bias){
	if(shadowFull) return windowSize/55.0;
	vec4 fragPos = shaderin.FragPosLightSpace;
	vec3 projCoords = fragPos.xyz/ fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;
	int count = 0;
	for(int i = -windowSize/2; i < windowSize/2; i++){
		for(int j = -windowSize/2; j < windowSize/2; j++){
			vec2 offset = vec2(i*1.0f,j * 1.0f)/shadowMapSize;
			float closestDepth = texture(depthMap, projCoords.xy + offset).r;
			int inShadow = currentDepth > (closestDepth + bias) ?  1 : 0;
			count += inShadow;
		}
	}


	return count/(windowSize*windowSize * 1.0f);
}
float shadowCalculation(vec4 fragPos, float distToLight, float bias){
	vec3 projCoords = fragPos.xyz/ fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float currentDepth = projCoords.z;


	int sampleWindow = 5;
	float depth = 0.0f;
	float blockCount = 0.0f;
	for(int i = -sampleWindow/2 ;i < sampleWindow/2; i++){
		for(int j = -sampleWindow/2 ; j < sampleWindow/2; j++){
			vec2 offset = vec2(i,j)/shadowMapSize;
			float closestDepth = texture(depthMap, projCoords.xy + offset).r;
			bool inShadow = currentDepth> (closestDepth + bias)? true : false;
			if(inShadow){
				depth = depth + closestDepth;
				blockCount = blockCount + 1;
			}
		}
	}
	
	if(blockCount  == 0.0f){
		return 0.0f;
	}
	float avgDepth = (depth/blockCount);
	float ratioToCurrentDepth = avgDepth/currentDepth;
	avgDepth = ratioToCurrentDepth * (distToLight);
	float penumbraSize = (distToLight - avgDepth) * lightSize * 5/avgDepth;
	penumbraSize = max(min(penumbraSize, 1.0f),0.1f);
	penumbraSize = penumbraSize*50 + 5;
	penumbraSize = int(penumbraSize) + int(penumbraSize)%2;
	int windowSize = int(penumbraSize);
	
	
	return calculatePercentage(windowSize, bias);
}

vec4 pointLight()
{	
	vec3 lightVec = lightPos - crntPos;

	float dist = length(lightVec);

	float a = .3;
	float b = 0.3;
	float inten = 2.0f/(a * dist * dist + b * dist + 1.0f);

	float ambient = 0.30f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection)/2, 0.0f);

	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.3f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 4);
		specular = specAmount * specularLight;
	};
	float cosTheta = dot(normal, lightDirection);
	float bias = 0.02*tan(acos(cosTheta));
	bias = clamp(bias, 0.03,0.2);
	float shadow = shadowCalculation(shaderin.FragPosLightSpace, dist, bias);
	shadow =0.0f;
	vec3 indirect = calculateIndirectLighting();
	if(isFloor) indirect = vec3(0.0f);

	return vec4((((color + indirect) * (diffuse * inten * (1.0 - shadow) + ambient) +  specular * (1.0-shadow) * inten)), 1.0f) * lightColor;
}

void main()
{
	// outputs final color
	FragColor = pointLight();
}