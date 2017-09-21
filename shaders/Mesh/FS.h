/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACGR.
 *
 * ACGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACGR.  If not, see <http://www.gnu.org/licenses/>.
 */
const char shader_mesh_fs_shadow[] = R"(
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
)";

const char shader_mesh_fs_light[] = R"(
vec4 ComputeLight(uint lightIndex, in Light light)
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
	diffuse = angle * vec4(light.diffuseColor, 1);

	//specular light
	dirToCamera = normalize(cameraPos - fragPos);
	halfwayDir = normalize(dirToLight + dirToCamera);

	angle2 = dot(normal, halfwayDir);
	angle2 = max(angle2, 0);

	specularIntensity = pow(angle2, 32); //32: shininess

	specular = specularIntensity * vec4(light.diffuseColor, 1);

	//compute result

	return (diffuse + specular) * light.power * attenuation * (1 - ComputeShadow(light, angle, lights[lightIndex].shadowMap));
}
)";

const char shader_mesh_material_color[] = R"(
vec4 ComputeMaterialColor()
{
	return vec4(material.diffuseColor, 1);
}
)";

const char shader_mesh_material_texture[] = R"(
vec4 ComputeMaterialColor()
{
	return texture(material.tex, fragTexCoords);
}
)";