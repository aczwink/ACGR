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
//Local
#include <ACGR/Renderer.hpp>
#include <ACGR/SceneNode.hpp>
#include "../Internal/ShaderCompiler.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGR
{
	class Ray
	{
	public:
		//Members
		float32 distance;
		const Mesh *pMesh;
		const Material *pMaterial;
		uint32 faceIndex;
		vec4f32 intersectionPoint;
		vec4f32 end; //in world coordinates
		vec4f32 normal;

		//Constructor
		inline Ray()
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

	class RayTraceRenderer : public Renderer
	{
	public:
		//Constructor
		RayTraceRenderer(DeviceContext &refDeviceContext);

		//Destructor
		~RayTraceRenderer();

		//Methods
		void EnableDebugMode(bool state);
		void InformDeviceStateChanged(const ACStdLib::Size &refSize);
		void RenderFrame(const SceneManager &refSceneMgr, const Camera &camera);

	private:
		//Members
		DeviceContext &refDeviceContext;
		ShaderCompiler shaderCompiler;
		ShaderProgram *program;
		InputState *pPlaneInputState;
		VertexBuffer *pPlaneVertexBuffer;
		Texture2D *pFrame;
		Vector3 *framePictureArray;
		Size frameSize;
		StaticThreadPool threadPool;

		//Methods
		vec4f32 ComputeLight(const Ray &refRay, const Light &refLight, const vec4f32 &refNormal);
		vec4f32 ComputeLights(const Ray &refRay, const vec4f32 &refNormal, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr);
		Vector3 ComputeShading(const Ray &refRay, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr);
		Vector3 ComputeSkyColor();
		void RenderLines(uint16 y, uint16 yMax, const Matrix4x4 &frustum, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr, const Camera &refCamera);
		void SetupPlane();
		void ShootRay(const Mesh &refMesh, const Material *pMaterial, const Matrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, Ray &refRay);
		void ShootRay(const SceneNode &refNode, const Matrix4x4 &refM, const vec4f32 &refOrigin, const vec4f32 &refDir, Ray &refRay);

		//Inline
		inline vec4f32 Transform(const vec4f32 &refCoef, const vec4f32 &refVec0, const vec4f32 &refVec1, const vec4f32 &refVec2, const Matrix4x4 &refTransform) const
		{
			return refCoef.x * (refTransform * refVec0) + refCoef.y * (refTransform * refVec1) + refCoef.z * (refTransform * refVec2);
		}
	};
}