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
//Local
#include <ACGR/Renderer.hpp>
#include <ACGR/SceneNode.hpp>
#include "../Internal/ShaderCompiler.hpp"
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::Rendering;

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
		Vector4S intersectionPoint;
		Vector4S end; //in world coordinates
		Vector4S normal;

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
		void InformDeviceStateChanged(const StdXX::Math::SizeS &refSize);
		void RenderFrame(const SceneManager &refSceneMgr, const Camera &camera);

	private:
		//Members
		DeviceContext &refDeviceContext;
		ShaderCompiler shaderCompiler;
		ShaderProgram *program;
		InputState *pPlaneInputState;
		VertexBuffer *pPlaneVertexBuffer;
		Texture2D *pFrame;
		Vector3S *framePictureArray;
		SizeS frameSize;
		StaticThreadPool threadPool;

		//Methods
		Vector4S ComputeLight(const Ray &refRay, const Light &refLight, const Vector4S &refNormal);
		Vector4S ComputeLights(const Ray &refRay, const Vector4S &refNormal, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr);
		Vector3S ComputeShading(const Ray &refRay, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr);
		Vector3S ComputeSkyColor();
		void RenderLines(uint16 y, uint16 yMax, const Matrix4S &frustum, const Map<Light *, SceneNode *> &refLights, const SceneManager &refSceneMgr, const Camera &refCamera);
		void SetupPlane();
		void ShootRay(const Mesh &refMesh, const Material *pMaterial, const Matrix4S &refM, const Vector4S &refOrigin, const Vector4S &refDir, Ray &refRay);
		void ShootRay(const SceneNode &refNode, const Matrix4S &refM, const Vector4S &refOrigin, const Vector4S &refDir, Ray &refRay);

		//Inline
		inline Vector4S Transform(const Vector4S &refCoef, const Vector4S &refVec0, const Vector4S &refVec1, const Vector4S &refVec2, const Matrix4S &refTransform) const
		{
			return refCoef.x * (refTransform * refVec0) + refCoef.y * (refTransform * refVec1) + refCoef.z * (refTransform * refVec2);
		}
	};
}