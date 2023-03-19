//Class Header
#include "CMatrix4x4.h"
//Namespaces
using namespace ACGE;

//Public methods
CMatrix4x4 CMatrix4x4::Transpose()
{
	__m128 tmp0, tmp1, tmp2, tmp3;
	CMatrix4x4 matrix;

	tmp0 = _mm_unpacklo_ps(this->rows[0].xmm, this->rows[1].xmm);
	tmp2 = _mm_unpacklo_ps(this->rows[2].xmm, this->rows[3].xmm);
	tmp1 = _mm_unpackhi_ps(this->rows[0].xmm, this->rows[1].xmm);
	tmp3 = _mm_unpackhi_ps(this->rows[2].xmm, this->rows[3].xmm);

	matrix[0].xmm = _mm_movelh_ps(tmp0, tmp2);
	matrix[1].xmm = _mm_movehl_ps(tmp2, tmp0);
	matrix[2].xmm = _mm_movelh_ps(tmp1, tmp3);
	matrix[3].xmm = _mm_movehl_ps(tmp3, tmp1);

	return matrix;
}

//Class Functions
CMatrix4x4 CMatrix4x4::Identity()
{
	CMatrix4x4 matrix;

	matrix.rows[0].e[0] = 1;
	matrix.rows[1].e[1] = 1;
	matrix.rows[2].e[2] = 1;
	matrix.rows[3].e[3] = 1;

	return matrix;
}

CMatrix4x4 CMatrix4x4::LookAtLH(const CVector3 &refEyePosition, const CVector3 &refFocusPosition, const CVector3 &refUpDirection)
{
	CVector3 xAxis, yAxis, zAxis;
	CMatrix4x4 matrix;

	//algorithm from: https://msdn.microsoft.com/en-us/library/windows/desktop/bb281710%28v=vs.85%29.aspx
	
	//zAxis is also the eye direction
	zAxis = (refFocusPosition - refEyePosition).GetNorm();
	xAxis = refUpDirection.Cross(zAxis).GetNorm();
	yAxis = zAxis.Cross(xAxis);
	
	matrix.rows[0] = CVector4(xAxis.x, yAxis.x, zAxis.x, 0);
	matrix.rows[1] = CVector4(xAxis.y, yAxis.y, zAxis.y, 0);
	matrix.rows[2] = CVector4(xAxis.z, yAxis.z, zAxis.z, 0);
	matrix.rows[3] = CVector4(-(xAxis * refEyePosition), -(yAxis * refEyePosition), -(zAxis * refEyePosition), 1);

	return matrix;
}

CMatrix4x4 CMatrix4x4::PerspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ)
{
	float32 xScale, yScale, range;
	CMatrix4x4 matrix;
	
	yScale = COT(fovAngleY / 2);
	xScale = yScale / aspectRatio;
	range = farZ / (farZ - nearZ);

	matrix.rows[0] = CVector4(xScale, 0, 0, 0);
	matrix.rows[1] = CVector4(0, yScale, 0, 0);
	matrix.rows[2] = CVector4(0, 0, range, 1);
	matrix.rows[3] = CVector4(0, 0, -nearZ * range, 0);
	
	return matrix;
}

CMatrix4x4 CMatrix4x4::RotationPitchYawRoll(float32 angleX, float32 angleY, float32 angleZ)
{
	return CMatrix4x4::RotationX(angleX) * CMatrix4x4::RotationY(angleY) * CMatrix4x4::RotationZ(angleZ);
}

CMatrix4x4 CMatrix4x4::RotationX(float32 angle)
{
	CMatrix4x4 matrix;

	matrix[0][0] = 1;
	matrix[1][1] = cos(angle);
	matrix[1][2] = sin(angle);
	matrix[2][1] = -matrix[1][2];
	matrix[2][2] = matrix[1][1];
	matrix[3][3] = 1;

	return matrix;
}

CMatrix4x4 CMatrix4x4::RotationY(float32 angle)
{
	CMatrix4x4 matrix;

	matrix[0][0] = cos(angle);
	matrix[2][0] = sin(angle);
	matrix[0][2] = -matrix[2][0];
	matrix[1][1] = 1;
	matrix[2][2] = matrix[0][0];
	matrix[3][3] = 1;
	
	return matrix;
}

CMatrix4x4 CMatrix4x4::RotationZ(float32 angle)
{
	CMatrix4x4 matrix;
	
	matrix[0][0] = cos(angle);
	matrix[0][1] = sin(angle);
	matrix[1][0] = -matrix[0][1];
	matrix[1][1] = matrix[0][0];
	matrix[2][2] = 1;
	matrix[3][3] = 1;
	
	return matrix;
}

CMatrix4x4 CMatrix4x4::Scale(float32 scaleX, float32 scaleY, float32 scaleZ)
{
	CMatrix4x4 matrix;

	matrix[0][0] = scaleX;
	matrix[1][1] = scaleY;
	matrix[2][2] = scaleZ;
	matrix[3][3] = 1.0f;

	return matrix;
}

CMatrix4x4 CMatrix4x4::Translation(float32 offsetX, float32 offsetY, float32 offsetZ)
{
	CMatrix4x4 matrix;

	matrix[0][0] = 1.0f;
	matrix[1][1] = 1.0f;
	matrix[2][2] = 1.0f;
	matrix[3] = CVector4(offsetX, offsetY, offsetZ, 1);

	return matrix;
}

//Matrix,Matrix operators
CMatrix4x4 ACGE::operator*(const CMatrix4x4 &refLeft, const CMatrix4x4 &refRight)
{
	CMatrix4x4 matrix;

	for(uint8 row = 0; row < 4; row++)
	{
		for(uint8 col = 0; col < 4; col++)
		{
			matrix[row][col] = 0;
			
			for(uint8 k = 0; k < 4; k++)
			{
				matrix[row][col] += refLeft[row][k] * refRight[k][col];
			}
		}
	}

	return matrix;
}