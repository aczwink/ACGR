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
#include <ACGR/SkyBox.hpp>
#include <ACGR/Texture.hpp>
#include "../Internal/ShaderCompiler.hpp"
#include "Passes/ShadowPass.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGR
{
	struct SLightInfo
	{
		Matrix4x4 lightVP;
		Texture2D *pShadowMap;
	};

	class DeviceRenderer : public Renderer
	{
		friend class ShadowPass;

		struct SMeshObjects
		{
			InputState *pInputState;
			VertexBuffer *pVertexBuffer;
			IndexBuffer *pIndexBuffer;
		};
	public:
		//Constructor
		DeviceRenderer(DeviceContext &deviceContext);

		//Destructor
		~DeviceRenderer();

		//Methods
		void RenderFrame(const SceneManager &refSceneMgr, const Camera &refCamera);

		//Inline
		inline DeviceContext &GetDeviceContext()
		{
			return this->refDeviceContext;
		}

		inline ShaderCompiler &GetShaderCompiler()
		{
			return this->shaderCompiler;
		}

	private:
		//Members
		Size deviceSize;
		DeviceContext &refDeviceContext;
		Matrix4x4 view;
		Matrix4x4 projection;
		Matrix4x4 VP;
		ShaderCompiler shaderCompiler;
		uint32 nLights;
		Map<const Light *, SLightInfo> additionalLightInfo;
		ShadowPass shadowPass;
		ShaderProgram *pMeshProgram;
		InputLayout meshInputLayout;
		Map<const Mesh *, SMeshObjects> meshObjects;
		Map<const Texture *, ITexture *> textures;
		struct
		{
			ShaderProgram *pFrameProgram;
			IFrameBuffer *pFrameBuffer;
			Texture2D *pFrameColorBuffer;
			InputState *pPlaneInputState;
			VertexBuffer *pPlaneVertexBuffer;
		};
		struct
		{
			SkyBox textures;
			ShaderProgram *pProgram;
			InputState *pBoxInputState;
			VertexBuffer *pBoxVertexBuffer;
			ICubeMap *pCubeMap;
		} skyBox;
		bool debugMode;
		struct
		{
			ShaderProgram *pNormalsProgram;
		} debug;

		//Methods
		void BeginRendering(const SceneManager &refSceneMgr, const Camera &refCamera);
		void EnableLight(const Light *pLight, bool state = true);
		void EndRendering();
		void InitPrograms();
		void Render(const Mesh *pMesh, const Matrix4x4 &refM);
		void Render(const SceneNode &refNode, const Matrix4x4 &refM);
		void RenderMesh(const Mesh *pMesh);
		void RenderShadowMap(const Light *pLight, const SceneNode &refNode, const Matrix4x4 &refM);
		void SetMaterial(const Material *pMaterial);
		void SetTexture(const Texture *pTexture);
		void SetupFrameBuffer();
		void SetupMeshObjects(const Mesh *pMesh, SMeshObjects &refMeshObjects);
		void SetSkyBox(const SkyBox &refSkyBox);
		void SetupSkyBox();

		//Inline
		inline void Render(const Entity &refEntity, const Matrix4x4 &refM)
		{
			if(refEntity.GetMaterial())
				this->SetMaterial(refEntity.GetMaterial());
			else
				this->SetMaterial(refEntity.GetMesh()->GetMaterial());

			this->Render(refEntity.GetMesh(), refM);
		}
	};
}