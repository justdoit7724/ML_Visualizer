#pragma once
#include "DXMesh.h"

namespace DX {

	class VertexLayout;

	class  CubeMesh :
		public Mesh
	{
	public:
		CubeMesh(ID3D11Device* device);
	};

}
