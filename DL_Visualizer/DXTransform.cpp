
#include "pch.h"

#include "DXTransform.h"
#include "DXMathHelper.h"

using namespace DX;

DX::Transform::Transform()
	:pos(0,0,0),right(RIGHT),up(UP),forward(FORWARD),scale(1,1,1)
{
}

XMMATRIX Transform::WorldMatrix()const
{
	return XMMATRIX(
		right.x * scale.x, right.y * scale.x, right.z * scale.x, 0,
		up.x * scale.y, up.y * scale.y, up.z * scale.y, 0,
		forward.x * scale.z, forward.y * scale.z, forward.z * scale.z, 0,
		pos.x, pos.y, pos.z, 1);
}

XMMATRIX Transform::S()const
{
	return XMMatrixScaling(scale.x, scale.y, scale.z);
}

XMMATRIX Transform::R()const
{
	return XMMATRIX(
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		0, 0, 0, 1);
}

XMMATRIX Transform::T()const
{
	return XMMatrixTranslation(pos.x, pos.y, pos.z);
}

void DX::Transform::SetRot(XMFLOAT3 _forward)
{
	forward = _forward;
	XMFLOAT3 tempUP = Equal(forward, UP) ? BACKWARD : UP;
	right = Normalize(Cross(tempUP, forward));
	up = Cross(forward, right);
}

void DX::Transform::SetRot(XMFLOAT3 _forward, XMFLOAT3 _up)
{
	forward = _forward;
	up = _up;

	float l1 = Length(forward);
	float l2 = Length(up);
	if (l1 < 0.9f || l1 > 1.1f || l2 < 0.9f || l2 > 1.1f)
		int a = 0;
	right = Cross(_up, _forward);
}

void DX::Transform::SetRot(XMFLOAT3 _forward, XMFLOAT3 _up, XMFLOAT3 _right)
{
	forward = _forward;
	up = _up;
	right = _right;
}

void Transform::Rotate(XMFLOAT3 axis, float rad)
{
	//  linear algebra
	//  calculated based from zero (local space)
	// apply formula for each default row of matrix

	float cosTheta = cos(rad);
	float sinTheta = sin(rad);

	XMMATRIX rotMat(
		(1.0f - cosTheta) * axis.x * axis.x + cosTheta,			 (1.0f - cosTheta) * axis.x * axis.y - sinTheta * axis.z, (1.0f - cosTheta) * axis.x * axis.z + sinTheta * axis.y, 0,
		(1.0f - cosTheta) * axis.x * axis.y + sinTheta * axis.z, (1.0f - cosTheta) * axis.y * axis.y + cosTheta,		  (1.0f - cosTheta) * axis.y * axis.z - sinTheta * axis.x, 0,
		(1.0f - cosTheta) * axis.x * axis.z - sinTheta * axis.y, (1.0f - cosTheta) * axis.y * axis.z + sinTheta * axis.x, (1.0f - cosTheta) * axis.z * axis.z + cosTheta, 0,
		0, 0, 0, 1);


	forward = MultiplyDir(forward, rotMat);
	up = MultiplyDir(up, rotMat);
	right = Cross(up, forward);
}
