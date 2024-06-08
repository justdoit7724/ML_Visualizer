
#include "pch.h"

#include "DXPhysic.h"
#include "DXObject.h"
#include "DXCollider.h"
#include "DXLayerMask.h"

using namespace DX;

Object* Physic::Raycast(const std::vector<Object*>& objs, Geometrics::Ray ray, int mask)
{
	/*
	for (Object* obj : objs)
	{
		if (!LayerCheck(obj->Layer(), mask))
			continue;

		XMFLOAT3 hitPt;
		if (obj->m_collider->IsHit(ray, &hitPt))
		{
			return obj;
		}
	}*/

	return nullptr;
}
