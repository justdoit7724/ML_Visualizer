
#include "pch.h"

#include "DXMesh.h"
#include "DXShaderFormat.h"
#include "DXLineMesh.h"

using namespace DX;

LineMesh::LineMesh()
{
	m_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}


