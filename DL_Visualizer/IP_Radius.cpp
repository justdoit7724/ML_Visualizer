#include "pch.h"
#include "IP_Radius.h"
#include "DXLib/MathHelper.h"

IP_Radius::IP_Radius(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int num)
	:InputPoints(origin, size,3)
{
	double longest = DX::Length(size) / 2;
	double rad = longest / 3;
	
	auto center1 = origin;
	center1.x += size.x / 3;
	center1.y += size.y / 3;
	center1.z += size.z / 3;
	auto center2 = origin;
	center2.x += 2*size.x / 3;
	center2.y += 2*size.y / 3;
	center2.z += 2*size.z / 3;


	// Generate points with labels
	for (int i = 0; i < num; ++i) {
		DirectX::XMFLOAT3 point;
		DirectX::XMFLOAT3 rpoint;
		rpoint.x = (rand() / (float)RAND_MAX-0.5) * size.x;
		rpoint.y = (rand() / (float)RAND_MAX-0.5) * size.y;
		rpoint.z = (rand() / (float)RAND_MAX-0.5) * size.z;
		point.x = origin.x + rpoint.x + size.x/2;
		point.y = origin.y + rpoint.y + size.y/2;
		point.z = origin.z + rpoint.z + size.z/2;


		auto diff1 = center1;
		diff1.x -= point.x;
		diff1.y -= point.y;
		diff1.z -= point.z;
		auto diff2 = center2;
		diff2.x -= point.x;
		diff2.y -= point.y;
		diff2.z -= point.z;
		double distance1 = DX::Length(diff1);
		double distance2 = DX::Length(diff2);
		int label = 0;
		if (distance1 < rad)
			label = 1;
		else if (distance2 < rad)
			label = 2;
		
		if (label >= 1)
		{
			point.x = origin.x + rpoint.x*0.7 + size.x / 2;
			point.y = origin.y + rpoint.y*0.7 + size.y / 2;
			point.z = origin.z + rpoint.z*0.7 + size.z / 2;
		}


		m_x.push_back({ point .x, point .y, point .z});
		m_y.push_back({ (double)label });

	}
}
