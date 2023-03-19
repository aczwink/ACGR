//Class header
#include "CRayTraceRenderer.h"
//Local
#include "../../../headers/Resources/CMaterial.h"
#include "../../../headers/Scene/CCamera.h"
#include "../../../headers/Scene/CSceneManager.h"
//Namespaces
using namespace ACGE;

//Constructor
CRayTraceRenderer::CRayTraceRenderer(CDeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext), shaderCompiler(refDeviceContext)
{
	this->pProgram = this->shaderCompiler.FetchStaticProgram("Frame");

	this->SetupPlane();
	
	this->pFrame = this->refDeviceContext.CreateTexture2D();
	this->pFrame->SetMaximumMipMapLevel(0);
	this->pFrameColorsArray = nullptr;
}

//Destructor
CRayTraceRenderer::~CRayTraceRenderer()
{
	if(this->pFrameColorsArray)
		delete[] this->pFrameColorsArray;
	delete this->pFrame;
	delete this->pPlaneInputState;
	delete this->pPlaneVertexBuffer;
}

//Private methods
vec4f32 CRayTraceRenderer::ComputeLight(const CRay &refRay, const CLight &refLight, const vec4f32 &refNormal)
{
	float32 attenuation, angle;
	vec4f32 dirToLight, diffuse, specular;

	//Blinn-Phong shading model

	//computations based on light type
	if(refLight.type == ELightType::Directional)
	{
		dirToLight = vec4f32(-refLight.direction, 0).Normalize();
		
		attenuation = 1.0f; //no attenuation over distance
	}
	else
	{
		float32 distance;
		
		distance = (vec4f32(refLight.position, 1) - refRay.end).Length();

		dirToLight = (vec4f32(refLight.position, 1) - refRay.end).Normalize();
		attenuation = 1.0f / (distance * distance); //amount of light diminishes with square of distance

		if(refLight.type == ELightType::Spot)
		{
			float32 lightAngle, delta, intensity, openingAngle;

			openingAngle = (float32)cos(CRadian(refLight.openingAngle).value);

			lightAngle = Dot(dirToLight, vec4f32(-refLight.direction, 0).Normalize());
			delta = (float32)cos(CRadian(refLight.innerConeAngle).value) - openingAngle;
			intensity = (lightAngle - openingAngle) / delta;
			intensity = CLAMP(intensity, 0, 1);

			attenuation *= intensity;
		}
	}

	//compute angle between normal and light direction (this equals the cosine in this case, because input vectors are normalized)
	angle = Dot(refNormal, dirToLight);
	angle = MAX(angle, 0); //if light is behind face (angle < 0), don't enlight
						   
	//diffuse light
	diffuse = angle * vec4f32(refLight.color, 1);

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

vec4f32 CRayTraceRenderer::ComputeLights(const CRay &refRay, const vec4f32 &refNormal, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr)
{
	vec4f32 combinedLight;

	combinedLight.w = 1;

	for(const auto &refKV : refLights)
	{
		combinedLight += this->ComputeLight(refRay, *refKV.key, refNormal);
	}

	return vec4f32(refSceneMgr.ambientLight, 1) + combinedLight;
}

CVector3 CRayTraceRenderer::ComputeShading(const CRay &refRay, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr)
{
	vec4f32 normal, materialColor, outColor;

	//mesh
	const CMesh::SVertex *const& refpVertices = refRay.pMesh->GetVertices();
	const uint16 *const& refpIndices16 = refRay.pMesh->GetIndices16();
	const CMesh::SVertex &refV0 = refpVertices[refRay.faceIndex];
	const CMesh::SVertex &refV1 = refpVertices[refRay.faceIndex+1];
	const CMesh::SVertex &refV2 = refpVertices[refRay.faceIndex+2];
	
	//compute normal
	normal = refRay.normal.Normalize();
	
	//get material color
	if(refRay.pMaterial)
	{
		materialColor = vec4f32(refRay.pMaterial->diffuseColor, 1);
	}


	//final mixing
	outColor = materialColor * this->ComputeLights(refRay, normal, refLights, refSceneMgr);

	return CVector3(outColor.x, outColor.y, outColor.z);
}

CVector3 CRayTraceRenderer::ComputeSkyColor()
{
	return CVector3();
}

void CRayTraceRenderer::SetupPlane()
{
	CInputLayout inputLayout;

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

void CRayTraceRenderer::ShootRay(const CMesh &refMesh, const CMaterial *pMaterial, const CMatrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, CRay &refRay)
{
	uint16 *pIndices;
	uint32 i;
	vec4f32 intersectionPoint, p0, p1, p2;//, endPoint;
	const CMesh::SVertex *pVertices;
	
	pIndices = refMesh.GetIndices16();
	pVertices = refMesh.GetVertices();
	for(i = 0; i < refMesh.GetNumberOfIndices(); i += 3)
	{
		const CMesh::SVertex &refV0 = pVertices[pIndices[i]];
		const CMesh::SVertex &refV1 = pVertices[pIndices[i+1]];
		const CMesh::SVertex &refV2 = pVertices[pIndices[i+2]];
		
		p0 = refM * vec4f32(refV0.position, 1);
		p1 = refM * vec4f32(refV1.position, 1);
		p2 = refM * vec4f32(refV2.position, 1);
		
		if(IntersectRayTriangle(refOrigin, refDir, p0, p1, p2, intersectionPoint))
		{
			if(intersectionPoint.z < refRay.distance) //test the depth value
			{
				CMatrix3x3 normalMatrix;
				
				//pre computations
				normalMatrix = refM.Inverse().Transpose();

				//set ray info
				refRay.distance = intersectionPoint.z;
				refRay.pMesh = &refMesh;
				refRay.pMaterial = pMaterial;
				refRay.faceIndex = i;
				refRay.intersectionPoint = intersectionPoint;
				refRay.end = intersectionPoint.x * p0 + intersectionPoint.y * p1 + intersectionPoint.z * p2;
				refRay.normal = this->Transform(intersectionPoint, vec4f32(refV0.normal, 0), vec4f32(refV1.normal, 0), vec4f32(refV2.normal, 0), normalMatrix);
			}
		}
	}
}

void CRayTraceRenderer::ShootRay(const CSceneNode &refNode, const CMatrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, CRay &refRay)
{
	AxisAlignedBox bbox;
	CMatrix4x4 transformation;

	transformation = refNode.GetTransformation() * refM;

	for(CEntity *pEntity : refNode.GetAttachedEntities())
	{
		const CMesh *const& refpMesh = pEntity->GetMesh();

		bbox = refpMesh->GetBoundingBox().Transform(transformation);

		if(bbox.Intersects(refOrigin, refDir) && bbox.SquaredDistanceTo(refOrigin) < refRay.distance*refRay.distance)
		{
			const CMaterial *const& refpMaterial = (pEntity->GetMaterial() ? pEntity->GetMaterial() : refpMesh->GetMaterial());

			this->ShootRay(*refpMesh, refpMaterial, transformation, refOrigin, refDir, refRay);
		}
	}

	for(CSceneNode *pChild : refNode.GetChildren())
	{
		this->ShootRay(*pChild, transformation, refOrigin, refDir, refRay);
	}
}

//Public methods
void CRayTraceRenderer::DeviceStateChanged(const ACStdLib::CSize &refSize)
{
	this->pFrame->AllocateRGB(refSize.width, refSize.height, nullptr);
	this->frameSize = refSize;

	if(this->pFrameColorsArray)
		delete[] this->pFrameColorsArray;
	this->pFrameColorsArray = new CVector3[refSize.width * refSize.height];
}

void CRayTraceRenderer::EnableDebugMode(bool state)
{
}

void CRayTraceRenderer::RenderFrame(const CSceneManager &refSceneMgr, const CCamera &refCamera)
{
	uint16 nLinesPerTask;
	uint32 i;
	CMatrix4x4 frustum;
	CMap<CLight *, CSceneNode *> lights;

	frustum = (refCamera.GetViewMatrix() * refCamera.GetPerspectiveMatrix()).Inverse();
	lights = refSceneMgr.GetLights();
	
	//generate tasks for thread pool
	nLinesPerTask = this->frameSize.height / this->threadPool.GetNumberOfThreads();
	for(i = 0; i < this->threadPool.GetNumberOfThreads(); i++)
	{
		this->threadPool.EnqueueTask([this, start = i * nLinesPerTask, end = (i + 1) * nLinesPerTask, nLinesPerTask, &frustum, &lights, &refSceneMgr, &refCamera]()
		{
			this->RenderLines(start, end, frustum, lights, refSceneMgr, refCamera);
		});
	}
	this->threadPool.EnqueueTask([this, start = i * nLinesPerTask, leftOver = this->frameSize.height, &frustum, &lights, &refSceneMgr, &refCamera]()
	{
		this->RenderLines(start, leftOver, frustum, lights, refSceneMgr, refCamera);
	});

	//wait for worker threads to render image
	this->threadPool.WaitForAllTasksToComplete();
	
	//update texture
	this->pFrame->UpdateRGB(0, 0, this->frameSize.width, this->frameSize.height, this->pFrameColorsArray);
	
	//render plane
	this->refDeviceContext.EnableDepthTest(false);

	this->refDeviceContext.SetInputState(this->pPlaneInputState);
	this->refDeviceContext.SetProgram(this->pProgram);
	this->refDeviceContext.SetTexture(0, this->pFrame);

	this->refDeviceContext.DrawTriangles(0, 2);
}

void CRayTraceRenderer::RenderLines(uint16 y, uint16 yMax, const CMatrix4x4 &refFrustum, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr, const CCamera &refCamera)
{
	uint16 x;
	uint32 index;
	vec4f32 start, end, dir;
	
	for(; y < yMax; y++)
	{
		for(x = 0; x < this->frameSize.width; x++)
		{
			CRay ray;
			
			//Normalized device coordinates
			start.x = -1 + 2 * (x / (float32)this->frameSize.width);
			start.y = -1 + 2 * (y / (float32)this->frameSize.height);
			start.z = -1;
			start.w = 1;

			end = start;
			end.z = 1;

			start = refFrustum * start;
			end = refFrustum * end;

			dir = (end / end.w - start / start.w).Normalize();
			
			this->ShootRay(*refSceneMgr.GetRootNode(), CMatrix4x4::Identity(), vec4f32(refCamera.position, 1), dir, ray);

			index = y * this->frameSize.width + x;
			if(ray.Hit())
			{
				this->pFrameColorsArray[index] = this->ComputeShading(ray, refLights, refSceneMgr);
			}
			else
				this->pFrameColorsArray[index] = this->ComputeSkyColor();
		}
	}
}