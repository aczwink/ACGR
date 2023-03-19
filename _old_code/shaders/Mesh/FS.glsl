#version 330 core
#extension GL_ARB_explicit_uniform_location : require

//Constants
const uint NUM_LIGHTS = 4u;
const float gamma = 2.2;

//Structs
struct Light
{
	uint type; //0 = Directional light, 1 = Point light, 2 = Spotlight
	vec3 color;
	float power;
	sampler2D shadowMap;
	mat4 lightVP;
	
	//For directional and Spotlight
	vec3 direction;
	//For Point and Spotlight
	vec3 pos;
	//For Spotlight
	float innerConeAngle;
	float openingAngle;
};

struct Material
{
	sampler2D tex;
};

//Uniforms
layout (location = 2) uniform vec3 ambientLight;
layout (location = 3) uniform vec3 cameraPos;
layout (location = 4) uniform samplerCube environment;
layout (location = 5) uniform uint maxLightIndex;
uniform Material material;
uniform Light lights[NUM_LIGHTS];

//Inputs
in vec3 fragPos; //in Worldspace
in vec3 fragNormal;
in vec2 fragTexCoords;

//Output
out vec4 outColor;

//Light functions
float ComputeShadow(in Light light, in float angleNormalAndLight, in sampler2D shadowMap)
{
	float depth, shadowDepth, shadow, bias;
	vec3 projCoords;
	vec4 fragPosLightSpace;
	
	fragPosLightSpace = light.lightVP * vec4(fragPos, 1);
	
	projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords / 2 + 0.5;
	
	shadowDepth = texture(shadowMap, projCoords.xy).r;
	depth = projCoords.z;
	
	bias = max(0.01 * (1.0 - angleNormalAndLight), 0.001);
	
	shadow = depth - bias > shadowDepth ? 1 : 0;
	
	//return 0;
	return shadow;
}

vec4 ComputeLight(in Light light)
{
	float angle, angle2, specularIntensity, attenuation;
	vec3 normal, dirToLight, dirToCamera, halfwayDir;
	vec4 diffuse, specular;
	
	//Blinn-Phong shading model
	
	//general computations
	normal = normalize(fragNormal);
	
	//computations based on light type
	if(light.type == 0u)
	{
		dirToLight = normalize(-light.direction);
		
		attenuation = 1.0f; //no attenuation over distance
	}
	else
	{
		float distance;
		
		distance = length(light.pos - fragPos);
		
		dirToLight = normalize(light.pos - fragPos);
		attenuation = 1.0f / (distance * distance); //amount of light diminishes with square of distance
		
		if(light.type == 2u)
		{
			float lightAngle, delta, intensity;
			
			lightAngle = dot(dirToLight, normalize(-light.direction));
			delta = light.innerConeAngle - light.openingAngle;
			intensity = (lightAngle - light.openingAngle) / delta;
			intensity = clamp(intensity, 0, 1);
			
			attenuation *= intensity;
		}
	}
	
	//compute angle between normal and light direction (this equals the cosine in this case, because input vectors are normalized)
	angle = dot(normal, dirToLight);
	angle = max(angle, 0); //if light is behind face (angle < 0), don't enlight
	
	//diffuse light
	diffuse = angle * vec4(light.color, 1);
	
	//specular light
	dirToCamera = normalize(cameraPos - fragPos);
	halfwayDir = normalize(dirToLight + dirToCamera);
	
	angle2 = dot(normal, halfwayDir);
	angle2 = max(angle2, 0);
	
	specularIntensity = pow(angle2, 32); //32: shininess
	
	specular = specularIntensity * vec4(light.color, 1);
	
	//compute result
	
	return (diffuse + specular) * light.power * attenuation * (1 - ComputeShadow(light, angle, lights[0].shadowMap)); //TODO: dynamic shadow maps-.-
}

vec4 ComputeLights()
{
	vec4 combinedLight;
	
	combinedLight = vec4(0, 0, 0, 1);
	
	for(uint i = 0u; i < maxLightIndex; i++)
	{
		combinedLight += ComputeLight(lights[i]);
	}
	
	return vec4(ambientLight, 1) + combinedLight;
}

void main()
{
	vec4 materialColor;
	
	materialColor = texture(material.tex, fragTexCoords);
	outColor = materialColor * ComputeLights();
	
	//apply gamma correction
	outColor.rgb = pow(outColor.rgb, vec3(1.0 / gamma));
}