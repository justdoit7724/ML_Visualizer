#pragma once

#include "DXMesh.h"

namespace DX {

	class  QuadMesh :
		public Mesh
	{
	public:
		QuadMesh(ID3D11Device* device);
	};
}

