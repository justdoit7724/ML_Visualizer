#pragma once

#include "DXMesh.h"

namespace DX
{

	class  CylinderMesh : public Mesh
	{
	public:
		CylinderMesh(ID3D11Device* device, int sCount);
	};
}