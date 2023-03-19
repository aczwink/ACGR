#pragma once
//Local
#include "CEntity.h"
#include "CLight.h"

namespace ACGE
{
	//Forward declarations
	class IRenderer;

	class ACGE_API CSceneNode
	{
		friend class CSceneManager;
	private:
		//Members
		CSceneNode *pParent;
		ACStdLib::Math::CMatrix4x4 transformation;
		ACStdLib::CLinkedList<CSceneNode *> children;
		ACStdLib::CLinkedList<CCamera *> attachedCameras;
		ACStdLib::CLinkedList<CEntity *> attachedEntities;
		ACStdLib::CLinkedList<CLight *> attachedLights;

		//Methods
		void Deserialize(const ACStdLib::XML::CElement &refElement);
		void GetLights(ACStdLib::CMap<CLight *, CSceneNode *> &refResult);
		float64 ParseAngle(const ACStdLib::CString &refString);
		void ParseRotation(const ACStdLib::XML::CElement &refElement);
	
	public:
		//Constructor
		CSceneNode();

		//Destructor
		~CSceneNode();

		//Methods
		ACStdLib::Math::CMatrix4x4 ComputeModelMatrix() const;

		//Inline
		inline void AddChild(CSceneNode *pNode)
		{
			this->children.InsertTail(pNode);
			pNode->pParent = this;
		}

		inline void AttachCamera(CCamera *pCamera)
		{
			this->attachedCameras.InsertTail(pCamera);
		}

		inline void AttachEntity(CEntity *pEntity)
		{
			this->attachedEntities.InsertTail(pEntity);
		}
		
		inline void AttachLight(CLight *pLight)
		{
			this->attachedLights.InsertTail(pLight);
		}

		inline const ACStdLib::CLinkedList<CCamera *> &GetAttachedCameras() const
		{
			return this->attachedCameras;
		}

		inline const ACStdLib::CLinkedList<CEntity *> &GetAttachedEntities() const
		{
			return this->attachedEntities;
		}

		inline const ACStdLib::CLinkedList<CLight *> &GetAttachedLights() const
		{
			return this->attachedLights;
		}

		inline const ACStdLib::CLinkedList<CSceneNode *> &GetChildren() const
		{
			return this->children;
		}

		inline ACStdLib::CMap<CLight *, CSceneNode *> GetLights()
		{
			ACStdLib::CMap<CLight *, CSceneNode *> result;
			this->GetLights(result);
			return result;
		}

		inline const ACStdLib::Math::CMatrix4x4 &GetTransformation() const
		{
			return this->transformation;
		}

		inline void Rotate(const ACStdLib::CRadian &refYaw, const ACStdLib::CRadian &refPitch, const ACStdLib::CRadian &refRoll)
		{
			this->transformation *= ACStdLib::Math::CMatrix4x4::RotationYawPitchRoll(refYaw, refPitch, refRoll);
		}

		inline void Scale(float32 scaleX, float32 scaleY, float32 scaleZ)
		{
			this->transformation *= ACStdLib::Math::CMatrix4x4::Scale(scaleX, scaleY, scaleZ);
		}

		inline void Translate(float32 dx, float32 dy, float32 dz)
		{
			this->transformation *= ACStdLib::Math::CMatrix4x4::Translation(dx, dy, dz);
		}

		//Functions
		static CSceneNode *Load(ACStdLib::AInputStream &refInput);
	};
}