#pragma once

#include "DXMesh.h"

namespace DX {

	class SphereMesh :
		public Mesh
	{
	public:
		SphereMesh(ID3D11Device* device, const int numSubDivision);
	};
}

