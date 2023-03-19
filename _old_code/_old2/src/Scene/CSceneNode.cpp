//Class Header
#include "../../headers/Scene/CSceneNode.h"
//Local
#include "../../headers/Scene/CFreeFlyCamera.h"
#include "../Internal/Parse.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::XML;
using namespace ACGE;

//Constructor
CSceneNode::CSceneNode()
{
	this->pParent = nullptr;
	this->transformation = CMatrix4x4::Identity();
}

//Destructor
CSceneNode::~CSceneNode()
{
	for(const CCamera *const& refpCamera : this->attachedCameras)
	{
		delete refpCamera;
	}

	for(const CEntity *const& refpEntity : this->attachedEntities)
	{
		delete refpEntity;
	}

	for(const CLight *const& refpLight : this->attachedLights)
	{
		delete refpLight;
	}

	for(const CSceneNode *const& refpChild : this->children)
	{
		delete refpChild;
	}
}

//Private methods
void CSceneNode::Deserialize(const CElement &refElement)
{
	CUTF8String text;

	for(const ANode *const& refpChild : refElement)
	{
		if(refpChild->GetType() == ENodeType::NODE_TYPE_ELEMENT)
		{
			const CElement &refChildElement = (const CElement &)*refpChild;

			if(refChildElement.GetName() == "Camera")
			{
				CCamera *pCamera;
				
				pCamera = new CFreeFlyCamera; //TODO: cam type
				this->AttachCamera(pCamera);

				if(refChildElement.HasAttribute("fovy"))
					pCamera->SetFieldOfView(this->ParseAngle(refChildElement.GetAttribute("fovy")));
				if(refChildElement.HasAttribute("position"))
					pCamera->position = ParseVec3(refChildElement.GetAttribute("position"));
				if(refChildElement.HasAttribute("direction"))
					pCamera->SetViewDirection(ParseVec3(refChildElement.GetAttribute("direction")));
				else if(refChildElement.HasAttribute("lookat"))
					pCamera->LookAt(ParseVec3(refChildElement.GetAttribute("lookat")));
			}
			else if(refChildElement.GetName() == "Entity")
			{
				CEntity *pEntity;
				CTextNode *pTextNode;

				pTextNode = (CTextNode *)refChildElement.GetChildren().Get(0);
				text = pTextNode->GetText();

				pEntity = new CEntity(C8BitString((char *)text.GetC_Str()));
				this->AttachEntity(pEntity);

				if(refChildElement.HasAttribute("material"))
					pEntity->SetMaterial(To8BitString(refChildElement.GetAttribute("material")));
			}
			else if(refChildElement.GetName() == "Light")
			{
				CLight *pLight;

				pLight = new CLight;
				
				if(refChildElement.GetAttribute("type") == "directional")
					pLight->type = ELightType::Directional;
				else if(refChildElement.GetAttribute("type") == "spot")
					pLight->type = ELightType::Spot;
				//else point

				if(refChildElement.HasAttribute("position"))
					pLight->position = ParseVec3(refChildElement.GetAttribute("position"));

				if(refChildElement.HasAttribute("direction"))
					pLight->direction = ParseVec3(refChildElement.GetAttribute("direction"));

				pLight->color = ParseVec3(refChildElement.GetAttribute("color"));
				
				pLight->power = (float32)StringToFloat64(refChildElement.GetAttribute("power"));

				if(refChildElement.HasAttribute("opening_angle"))
					pLight->openingAngle = this->ParseAngle(refChildElement.GetAttribute("opening_angle"));
				if(refChildElement.HasAttribute("inner_cone_angle"))
					pLight->innerConeAngle = this->ParseAngle(refChildElement.GetAttribute("inner_cone_angle"));
				
				//advanced values
				if(refChildElement.HasAttribute("lookat"))
				{
					CVector3 target;

					target = ParseVec3(refChildElement.GetAttribute("lookat"));

					pLight->LookAt(target);
				}

				this->AttachLight(pLight);
			}
			else if(refChildElement.GetName() == "Node")
			{
				CSceneNode *pNode;
				CVector3 v;

				pNode = new CSceneNode;
				pNode->Deserialize(refChildElement);

				//first scale
				if(refChildElement.HasAttribute("scale"))
				{
					v = ParseVec3(refChildElement.GetAttribute("scale"));
					pNode->Scale(v.x, v.y, v.z);
				}

				//then rotate
				pNode->ParseRotation(refChildElement);

				//then translate
				if(refChildElement.HasAttribute("translate"))
				{
					v = ParseVec3(refChildElement.GetAttribute("translate"));
					pNode->Translate(v.x, v.y, v.z);
				}

				this->AddChild(pNode);
			}
		}
	}
}

void CSceneNode::GetLights(CMap<CLight *, CSceneNode *> &refResult)
{
	for(CLight *const& refpLight : this->attachedLights)
	{
		refResult.Insert(refpLight, this);
	}
	
	for(CSceneNode *const& refpChild : this->children)
	{
		refpChild->GetLights(refResult);
	}
}

float64 CSceneNode::ParseAngle(const CString &refString)
{
	if(refString.IsEmpty())
		return 0;

	if(refString[refString.GetLength() - 1] == 0xB0) //degree sign
	{
		CString tmp;

		tmp = refString.SubString(0, refString.GetLength() - 1);

		return StringToFloat64(tmp);
	}

	return CDegree(CRadian(StringToFloat64(refString))).value;
}

void CSceneNode::ParseRotation(const CElement &refElement)
{
	CVector3 v;

	if(refElement.HasAttribute("rotate_ypr"))
	{
		v = ParseVec3(refElement.GetAttribute("rotate_ypr"));
		this->Rotate(CDegree(v.x), CDegree(v.y), CDegree(v.z));
	}
}

//Public methods
CMatrix4x4 CSceneNode::ComputeModelMatrix() const
{
	CSceneNode *pNode;
	CMatrix4x4 model;
	
	model = this->transformation;
	pNode = this->pParent;
	while(pNode)
	{
		model = pNode->transformation * model;
		pNode = pNode->pParent;
	}

	return model;
}

//Class Functions
CSceneNode *CSceneNode::Load(AInputStream &refInput)
{
	CSceneNode *pNode;
	XML::CDocument *pDocument;

	pDocument = XML::CDocument::Parse(refInput);
	pNode = new CSceneNode;

	pNode->Deserialize(pDocument->GetRootElement());

	delete pDocument;

	return pNode;
}