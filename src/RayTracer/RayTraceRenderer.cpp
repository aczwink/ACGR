/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Class header
#include "RayTraceRenderer.hpp"
//Local
#include <ACGR/Camera.hpp>
#include <ACGR/SceneManager.hpp>
#include "../../shaders/Frame.h"
//Namespaces
using namespace ACGR;

//Constructor
RayTraceRenderer::RayTraceRenderer(DeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext), shaderCompiler(refDeviceContext)
{
	this->program = this->shaderCompiler.CompileStaticProgram(SHADER_ARGS(frame));

	this->SetupPlane();

	this->pFrame = this->refDeviceContext.CreateTexture2D();
	this->pFrame->SetMaximumMipMapLevel(0);
	this->framePictureArray = nullptr;
}

//Destructor
RayTraceRenderer::~RayTraceRenderer()
{
	if(this->framePictureArray)
		delete[] this->framePictureArray;
	delete this->pFrame;
	delete this->pPlaneInputState;
	delete this->pPlaneVertexBuffer;
}

//Public methods
void RayTraceRenderer::EnableDebugMode(bool state)
{
}
void RayTraceRenderer::InformDeviceStateChanged(const StdXX::Math::SizeS &refSize)
{
	this->pFrame->AllocateRGB(refSize.Cast<uint16>(), nullptr);
	this->frameSize = refSize;

	if(this->framePictureArray)
		delete[] this->framePictureArray;
	this->framePictureArray = new Vector3S[refSize.width * refSize.height];
}

void RayTraceRenderer::RenderFrame(const SceneManager &refSceneMgr, const Camera &camera)
{
	uint16 nLinesPerTask;
	uint32 i;
	Matrix4S frustum;
	Map<Light *, SceneNode *> lights;

	frustum = (camera.GetPerspectiveMatrix() * camera.GetViewMatrix()).Inverse();
	lights = refSceneMgr.GetLights();

	//generate tasks for thread pool
	nLinesPerTask = static_cast<uint16>(this->frameSize.height / this->threadPool.GetNumberOfThreads());
	for(i = 0; i < this->threadPool.GetNumberOfThreads(); i++)
	{
		this->threadPool.EnqueueTask(
			[this, start = i * nLinesPerTask, end = (i + 1) * nLinesPerTask, nLinesPerTask, &frustum, &lights, &refSceneMgr, &camera]()
			{
				this->RenderLines(start, end, frustum, lights, refSceneMgr, camera);
			}
		);
	}
	this->threadPool.EnqueueTask(
		[this, start = i * nLinesPerTask, leftOver = this->frameSize.height, &frustum, &lights, &refSceneMgr, &camera]()
		{
			this->RenderLines(start, leftOver, frustum, lights, refSceneMgr, camera);
		}
	);

	//wait for worker threads to render image
	this->threadPool.WaitForAllTasksToComplete();

	//update texture
	this->pFrame->UpdateRGB(0, 0, this->frameSize.width, this->frameSize.height, this->framePictureArray);

	FileOutputStream fos(Path("blub.ppm"));
	DataWriter writer(true, fos);
	fos << "P6" << endl << this->frameSize.width << " " << this->frameSize.height << endl << "255" << endl;
	for(int y = this->frameSize.height-1; y >= 0; y--)
	{
		for(int x = 0; x < this->frameSize.width; x++)
		{
			int i = y * this->frameSize.width + x;
			writer.WriteByte(static_cast<byte>(this->framePictureArray[i].z * 255));
			writer.WriteByte(static_cast<byte>(this->framePictureArray[i].y * 255));
			writer.WriteByte(static_cast<byte>(this->framePictureArray[i].x * 255));
		}
	}

	//render plane
	this->refDeviceContext.EnableDepthTest(false);

	this->refDeviceContext.SetInputState(this->pPlaneInputState);
	this->refDeviceContext.SetProgram(this->program);
	this->refDeviceContext.SetTexture(0, this->pFrame);

	this->refDeviceContext.DrawTriangles(0, 2);
}

//Private methods
Vector4S RayTraceRenderer::ComputeLight(const Ray &refRay, const Light &refLight, const Vector4S &refNormal)
{
	float32 attenuation, angle;
	Vector4S dirToLight, diffuse, specular;

	//Blinn-Phong shading model

	//computations based on light type
	if(refLight.type == ELightType::Directional)
	{
		dirToLight = Vector4S(-refLight.direction, 0).Normalized();

		attenuation = 1.0f; //no attenuation over distance
	}
	else
	{
		float32 distance;

		distance = (Vector4S(refLight.position, 1) - refRay.end).Length();

		dirToLight = (Vector4S(refLight.position, 1) - refRay.end).Normalized();
		attenuation = 1.0f / (distance * distance); //amount of light diminishes with square of distance

		if(refLight.type == ELightType::Spot)
		{
			float32 lightAngle, delta, intensity, openingAngle;

			openingAngle = (float32)cos(Radian(refLight.openingAngle).value);

			lightAngle = dirToLight.Dot(Vector4S(-refLight.direction, 0).Normalized());
			delta = (float32)cos(Radian(refLight.innerConeAngle).value) - openingAngle;
			intensity = (lightAngle - openingAngle) / delta;
			intensity = Clamp(intensity, 0.0f, 1.0f);

			attenuation *= intensity;
		}
	}

	//compute angle between normal and light direction (this equals the cosine in this case, because input vectors are normalized)
	angle = refNormal.Dot(dirToLight);
	angle = Max(angle, 0.0f); //if light is behind face (angle < 0), don't enlight

	//diffuse light
	diffuse = angle * Vector4S(refLight.color, 1);

	/*
	float32 angle2, specularIntensity;
	//vec3 normal, dirToCamera, halfwayDir;

	//specular light
	dirToCamera = normalize(cameraPos - fragPos);
	halfwayDir = normalize(dirToLight + dirToCamera);

	angle2 = dot(normal, halfwayDir);
	angle2 = max(angle2, 0);

	specularIntensity = pow(angle2, 32); //32: shininess

	specular = specularIntensity * light.color;

	//compute result
	*/

	return (diffuse + specular) * refLight.power * attenuation;
}

Vector4S RayTraceRenderer::ComputeLights(const Ray &refRay, const Vector4S &refNormal, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr)
{
	Vector4S combinedLight;

	combinedLight.w = 1;

	for(const auto &refKV : refLights)
	{
		combinedLight += this->ComputeLight(refRay, *refKV.key, refNormal);
	}

	return Vector4S(refSceneMgr.ambientLight, 0) + combinedLight;
}

Vector3S RayTraceRenderer::ComputeShading(const Ray &refRay, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr)
{
	Vector4S normal, materialColor, outColor;

	//mesh
	const Mesh::Vertex *const& refpVertices = refRay.pMesh->GetVertices();
	const uint16 *const& refpIndices16 = refRay.pMesh->GetIndices16();
	const Mesh::Vertex &refV0 = refpVertices[refRay.faceIndex];
	const Mesh::Vertex &refV1 = refpVertices[refRay.faceIndex+1];
	const Mesh::Vertex &refV2 = refpVertices[refRay.faceIndex+2];

	//compute normal
	normal = refRay.normal.Normalized();

	//get material diffuseColor
	if(refRay.pMaterial)
	{
		materialColor = Vector4S(refRay.pMaterial->diffuseColor, 1);
	}


	//final mixing
	outColor = materialColor * this->ComputeLights(refRay, normal, refLights, refSceneMgr);

	return Vector3(outColor.x, outColor.y, outColor.z);
}

Vector3S RayTraceRenderer::ComputeSkyColor()
{
	return Vector3S();
}

void RayTraceRenderer::RenderLines(uint16 y, uint16 yMax, const Matrix4S &frustum, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr, const Camera &refCamera)
{
	uint16 x;
	uint32 index;
	Vector4S start, end, dir;

	for(; y < yMax; y++)
	{
		for(x = 0; x < this->frameSize.width; x++)
		{
			Ray ray;

			//Normalized device coordinates
			start.x = -1 + 2 * (x / (float32)this->frameSize.width);
			start.y = -1 + 2 * (y / (float32)this->frameSize.height);
			start.z = -1; //NDC is a left-handed coordinate system
			start.w = 1;

			end = start;
			end.z = 1;

			start = frustum * start;
			end = frustum * end;

			dir = (end / end.w - start / start.w).Normalized();

			this->ShootRay(*refSceneMgr.GetRootNode(), Matrix4S::Identity(), Vector4S(refCamera.position, 1), dir, ray);

			index = y * this->frameSize.width + x;
			if(ray.Hit())
			{
				this->framePictureArray[index] = this->ComputeShading(ray, refLights, refSceneMgr);
			}
			else
				this->framePictureArray[index] = this->ComputeSkyColor();
		}
	}
}

void RayTraceRenderer::SetupPlane()
{
	InputLayout inputLayout;

	static const float32 planePositions[][2] =
	{
		//lower left
		{-1, 1}, //top
		{-1, -1}, //bottom
		{1, -1}, //right

		//upper right
		{1, 1}, //top
		{-1, 1}, //left
		{1, -1}, //bottom
	};

	//create input layout
	inputLayout.AddAttribute2();

	//plane vertex buffer
	this->pPlaneVertexBuffer = this->refDeviceContext.CreateVertexBuffer();
	this->pPlaneVertexBuffer->Allocate(6, sizeof(planePositions[0]), planePositions);

	//plane input state
	this->pPlaneInputState = this->refDeviceContext.CreateInputState();
	this->pPlaneInputState->AddVertexBuffer(this->pPlaneVertexBuffer, inputLayout);
}

void RayTraceRenderer::ShootRay(const Mesh &refMesh, const Material *pMaterial, const Matrix4S &refM, const Vector4S &refOrigin, const Vector4S &refDir, Ray &refRay)
{
	uint16 *pIndices;
	uint32 i;
	Vector4S intersectionPoint, p0, p1, p2;//, endPoint;
	const Mesh::Vertex *pVertices;

	pIndices = refMesh.GetIndices16();
	pVertices = refMesh.GetVertices();
	for(i = 0; i < refMesh.GetNumberOfIndices(); i += 3)
	{
		const Mesh::Vertex &refV0 = pVertices[pIndices[i]];
		const Mesh::Vertex &refV1 = pVertices[pIndices[i+1]];
		const Mesh::Vertex &refV2 = pVertices[pIndices[i+2]];

		p0 = refM * Vector4S(refV0.position, 1);
		p1 = refM * Vector4S(refV1.position, 1);
		p2 = refM * Vector4S(refV2.position, 1);

		if(IntersectRayTriangle(refOrigin, refDir, p0, p1, p2, intersectionPoint))
		{
			if(intersectionPoint.z < refRay.distance) //test the depth value
			{
				Matrix3S normalMatrix;

				//pre computations
				normalMatrix = refM.Inverse().Transpose();

				//set ray info
				refRay.distance = intersectionPoint.z;
				refRay.pMesh = &refMesh;
				refRay.pMaterial = pMaterial;
				refRay.faceIndex = i;
				refRay.intersectionPoint = intersectionPoint;
				refRay.end = intersectionPoint.x * p0 + intersectionPoint.y * p1 + intersectionPoint.z * p2;
				refRay.normal = this->Transform(intersectionPoint, Vector4S(refV0.normal, 0), Vector4S(refV1.normal, 0), Vector4S(refV2.normal, 0), normalMatrix);
			}
		}
	}
}

void RayTraceRenderer::ShootRay(const SceneNode &refNode, const Matrix4S &refM, const Vector4S &refOrigin, const Vector4S &refDir, Ray &refRay)
{
	AxisAlignedBoxS bbox;
	Matrix4S transformation;

	transformation = refNode.GetTransformation() * refM;

	for(Entity *pEntity : refNode.GetAttachedEntities())
	{
		const Mesh *const& refpMesh = pEntity->GetMesh();

		bbox = refpMesh->GetBoundingBox().Transform(transformation);

		if(bbox.Intersects(refOrigin, refDir))
		{
			const Material *const& refpMaterial = (pEntity->GetMaterial() ? pEntity->GetMaterial() : refpMesh->GetMaterial());

			this->ShootRay(*refpMesh, refpMaterial, transformation, refOrigin, refDir, refRay);
		}
	}

	for(SceneNode *pChild : refNode.GetChildren())
	{
		this->ShootRay(*pChild, transformation, refOrigin, refDir, refRay);
	}
}