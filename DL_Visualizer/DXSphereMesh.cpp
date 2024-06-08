
#include "pch.h"

#include "DXSphereMesh.h"
#include "DXShaderFormat.h"
#include "DXMathHelper.h"

using namespace DX;

inline void Subdivide(std::vector<XMFLOAT3>& vertice, std::vector<int>& indice)
{
	std::vector<XMFLOAT3> tempVertice(vertice);
	std::vector<int> tempIndice(indice);
	vertice.clear();
	indice.clear();
	int idx = 0;
	for (int i = 0; i < tempIndice.size(); i += 3)
	{
		XMFLOAT3 v1 = tempVertice[tempIndice[i]];
		XMFLOAT3 v2 = tempVertice[tempIndice[i + 1]];
		XMFLOAT3 v3 = tempVertice[tempIndice[i + 2]];

		XMFLOAT3 nv1 = Normalize( (v1+ v3) / 2)/2;
		XMFLOAT3 nv2 = Normalize( (v1+ v2) / 2)/2;
		XMFLOAT3 nv3 = Normalize( (v3+ v2) / 2)/2;


		// compute 3 new vertices by spliting half on each edge
		//         v1       
		//        / \       
		//   nV1 *---* nV2
		//      / \ / \     
		//    v3---*---v2   
		//        nV3    

		vertice.push_back(v1);
		vertice.push_back(nv1);
		vertice.push_back(nv2);
		vertice.push_back(v3);
		vertice.push_back(nv3);
		vertice.push_back(v2);

		indice.push_back(idx), indice.push_back(idx + 2), indice.push_back(idx + 1);
		indice.push_back(idx + 1), indice.push_back(idx + 2), indice.push_back(idx + 4);
		indice.push_back(idx + 3), indice.push_back(idx + 1), indice.push_back(idx + 4);
		indice.push_back(idx + 4), indice.push_back(idx + 2), indice.push_back(idx + 5);
		idx += 6;
	}
}

SphereMesh::SphereMesh(ID3D11Device* device, int numSubDivision)
	:Mesh()
{
	// Put a cap on the number of subdivisions. 
	numSubDivision = fminf(numSubDivision, 3u);

	// Approximate a sphere by tessellating an icosahedron. 
	const auto stdPt = Normalize(XMFLOAT3(0.525731f,0, 0.850651f))/2;
	XMFLOAT3 pos[12] = {
		XMFLOAT3(-stdPt.x, 0.0f, stdPt.z),
		XMFLOAT3(stdPt.x, 0.0f, stdPt.z),
		XMFLOAT3(-stdPt.x, 0.0f, -stdPt.z),
		XMFLOAT3(stdPt.x, 0.0f, -stdPt.z),
		XMFLOAT3(0.0f, stdPt.z, stdPt.x),
		XMFLOAT3(0.0f, stdPt.z, -stdPt.x),
		XMFLOAT3(0.0f, -stdPt.z, stdPt.x),
		XMFLOAT3(0.0f, -stdPt.z, -stdPt.x),
		XMFLOAT3(stdPt.z, stdPt.x, 0.0f),
		XMFLOAT3(-stdPt.z, stdPt.x, 0.0f),
		XMFLOAT3(stdPt.z, -stdPt.x, 0.0f),
		XMFLOAT3(-stdPt.z, -stdPt.x, 0.0f)
	};
	UINT k[60] = {
		1,4,0, 4,9,0,
		4,5,9, 8,5,4,
		1,8,4, 1,10,8,
		10,3,8, 8,3,5,
		3,2,5, 3,7,2,
		3,10,7, 10,6,7,
		6,11,7, 6,0,11,
		6,1,0, 10,1,6,
		11,0,9, 2,11,9,
		5,2,9, 11,2,7
	};
	std::vector<XMFLOAT3> vertice(12);
	std::vector<int> indice(60);
	for (size_t i = 0; i < 12; ++i)
		vertice[i] = pos[i];
	for (size_t i = 0; i < 60; ++i)
		indice[i] = k[i];
	for (size_t i = 0; i < numSubDivision; ++i)
		Subdivide(vertice, indice);
	// Project vertices onto sphere and scale. 

	/**/
	m_vertice.resize(vertice.size());
	for (size_t i = 0; i < vertice.size(); ++i) {
		
		XMFLOAT3 pos = vertice[i];
		XMFLOAT3 n = Normalize(pos);

		// Derive texture coordinates from spherical coordinates.
		float theta = atan2(pos.x, pos.z) + XM_PI;
		float phi = acosf(pos.y);
		XMFLOAT2 tex = XMFLOAT2(theta / XM_2PI, phi / XM_PI);
		/*
		XMFLOAT3 tangent = XMFLOAT3(
			cosf(theta),
			0.0f,
			-sinf(theta)
		);*/

		m_vertice[i].pos = pos;

		m_vertice[i].tex= tex;
		
		m_vertice[i].normal = n;
	}

	int* indiceData = indice.data();

	SetIndice(indiceData, indice.size());
	Update(device);
}



