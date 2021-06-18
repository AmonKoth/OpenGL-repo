#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;

out vec4 color;

const int MAX_POINT_LIGHTS =3;

struct Light
{
	vec3 color;
	float ambientIntensity;

	float diffuseIntensity;

};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;

};


struct Material
{
	float specularInt;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform Material material;
uniform sampler2D theTexture;

uniform vec3 cameraPos;

vec4 CalcLightByDirection(Light light,vec3 dir)
{
	vec4 ambientColor = vec4(light.color,1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal),normalize(dir)),0.0f);
	vec4 diffuseColor = vec4(light.color,1.0f)*light.diffuseIntensity* diffuseFactor;
	
	vec4 specularColor = vec4(0,0,0,0);
	
	if(diffuseFactor>0.0f)
	{
		vec3 fragToCam = normalize(cameraPos - fragPos);
		vec3 reflectedVertex = normalize(reflect(dir, normalize(Normal)));
		
		float specularFactor = dot(fragToCam,reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color* material.specularInt * specularFactor,1.0f); 
		}
	}
	return (ambientColor+diffuseColor+specularColor);
}


vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base , directionalLight.direction);
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0,0,0,0);
	for(int i=0; i<pointLightCount; i++)
	{
		vec3 direction = fragPos - pointLights[i].position; 
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightByDirection(pointLights[i].base,direction);
		float attenuation = pointLights[i].exponent *distance * distance + 
		pointLights[i].linear * distance+
		pointLights[i].constant;
		
		totalColor += (color/attenuation);
	}
	return totalColor;
}

void main ()
{

	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	
	color = texture(theTexture,TexCoord) *finalColor;
}