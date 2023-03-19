//Local
#include "CMatrix4x4.h"
#include "CVector3.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CCamera
	{
	private:
		//Variables
		CVector3 position;
		CVector3 rotation;
		CMatrix4x4 viewMatrix;
	public:
		//Functions
		const CVector3 &GetPosition() const;
		const CVector3 &GetRotation() const;
		const CMatrix4x4 &GetViewMatrix() const;
		void SetPosition(const CVector3 &position);
		void SetRotation(const CVector3 &rotation);
		void Update();
	};
}