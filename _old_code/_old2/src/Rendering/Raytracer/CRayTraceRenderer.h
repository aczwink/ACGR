#pragma once
//Local
#include "../../../headers/Rendering/IRenderer.h"
#include "../../Internal/CShaderCompiler.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGE
{
	//Forward declarations
	class CSceneNode;

	class CRayTraceRenderer : public IRenderer
	{
		class CRay
		{
		public:
			//Members
			float32 distance;
			const CMesh *pMesh;
			const CMaterial *pMaterial;
			uint32 faceIndex;
			vec4f32 intersectionPoint;
			vec4f32 end; //in world coordinates
			vec4f32 normal;

			//Constructor
			inline CRay()
			{
				this->distance = INFINITY;
				this->pMesh = nullptr;
			}

			//Inline
			inline bool Hit() const
			{
				return this->pMesh != nullptr;
			}
		};
	private:
		//Members
		CShaderCompiler shaderCompiler;
		IShaderProgram *pProgram;
		IInputState *pPlaneInputState;
		IVertexBuffer *pPlaneVertexBuffer;
		ITexture2D *pFrame;
		CVector3 *pFrameColorsArray;
		CSize frameSize;

		CDeviceContext &refDeviceContext;

		CStaticThreadPool threadPool;

		//Methods
		vec4f32 ComputeLight(const CRay &refRay, const CLight &refLight, const vec4f32 &refNormal);
		vec4f32 ComputeLights(const CRay &refRay, const vec4f32 &refNormal, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr);
		CVector3 ComputeShading(const CRay &refRay, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr);
		CVector3 ComputeSkyColor();
		void SetupPlane();
		void ShootRay(const CMesh &refMesh, const CMaterial *pMaterial, const CMatrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, CRay &refRay);
		void ShootRay(const CSceneNode &refNode, const CMatrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, CRay &refRay);

		//Inline
		inline vec4f32 Transform(const vec4f32 &refCoef, const vec4f32 &refVec0, const vec4f32 &refVec1, const vec4f32 &refVec2, const CMatrix4x4 &refTransform) const
		{
			return refCoef.x * (refTransform * refVec0) + refCoef.y * (refTransform * refVec1) + refCoef.z * (refTransform * refVec2);
		}

	public:
		//Constructor
		CRayTraceRenderer(CDeviceContext &refDeviceContext);

		//Destructor
		~CRayTraceRenderer();

		//Methods
		void DeviceStateChanged(const ACStdLib::CSize &refSize);
		void EnableDebugMode(bool state);
		void RenderFrame(const CSceneManager &refSceneMgr, const CCamera &refCamera);
		void RenderLines(uint16 y, uint16 yMax, const CMatrix4x4 &refFrustum, const CMap<CLight *, CSceneNode *> &refLights, const CSceneManager &refSceneMgr, const CCamera &refCamera);
	};
}