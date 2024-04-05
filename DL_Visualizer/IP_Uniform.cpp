#include "pch.h"
#include "IP_Uniform.h"
#include <random>

IP_Uniform::IP_Uniform(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int nHor, int nVer)
	:InputPoints(origin, size,4)
{
	float distScale = 25;

	// Generate points with labels
	for (int z = 0; z < nHor; ++z)
	{
		for (int y = 0; y < nVer; ++y)
		{
			for (int x = 0; x < nHor; ++x)
			{
				DirectX::XMFLOAT3 norm = { x/ float(nHor-1)-0.5f,y/ float(nVer-1)-0.5f,z/ float(nHor-1)-0.5f };
				
				int label = 0;
				if (norm.x > 0 && norm.z > 0)
				{
					label = 0; 
					norm.x += size.x / distScale;
					norm.z += size.z / distScale;
				}
				else if (norm.x > 0 && norm.z < 0)
				{
					label = 1;
					norm.x += size.x / distScale;
					norm.z -= size.z / distScale;
				}
				else if (norm.x < 0 && norm.z < 0)
				{
					label = 2;
					norm.x -= size.x / distScale;
					norm.z -= size.z / distScale;
				}
				else 
				{
					label = 3;
					norm.x -= size.x / distScale;
					norm.z += size.z / distScale;
				}

				DirectX::XMFLOAT3 pt;
				pt.x = norm.x*size.x+size.x/2 + origin.x;
				pt.y = norm.y*size.y+size.y/2 + origin.y;
				pt.z = norm.z*size.z+size.z/2 + origin.z;

				m_x.push_back({ pt.x, pt.y, pt.z });
				m_y.push_back({ (double)label });
			}
		}
	}
}
