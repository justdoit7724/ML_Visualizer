
#include "pch.h"

#include "DXSphereCollider.h"

using namespace DX;

SphereCollider::SphereCollider()
	:Collider(XMFLOAT3(0,0,0)), rad(0.5f)
{
}

SphereCollider::SphereCollider(XMFLOAT3 center, float rad)
	:Collider(center), rad(rad)
{
}

bool SphereCollider::IsHit(Geometrics::Ray ray, XMFLOAT3* hitPt)
{
	*hitPt = XMFLOAT3(0, 0, 0);

	return IntersectRaySphere(ray, Geometrics::Sphere(center, rad));
}

void SphereCollider::SetScale(XMFLOAT3 s)
{
	rad = s.x * 0.5f;
}

void SphereCollider::Visualize()
{
	//Debugging::Instance()->Mark(center, rad, Colors::LightGreen);
}
