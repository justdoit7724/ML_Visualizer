#include "pch.h"
#include "IP_Simple.h"
#include <random>

IP_Simple::IP_Simple(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int num, int nLabel)
	:InputPoints(origin, size, nLabel)
{
    m_centers.clear();
    m_centers.push_back(origin);
    m_centers.back().x += size.x / 4;
	m_centers.back().y += size.y / 4;
	m_centers.back().z += size.z / 4;

    m_centers.push_back(origin);
    m_centers.back().x += 3*size.x / 4;
    m_centers.back().y += 3*size.y / 4;
    m_centers.back().z += 3*size.z / 4;

    m_centers.push_back(origin);
    m_centers.back().x += 3 * size.x / 4;
    m_centers.back().y -= 2 * size.y / 4;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distX(0, size.x / 8); // Gaussian distribution with mean 0 and standard deviation radius/3
    std::normal_distribution<> distY(0, size.y / 8); // Gaussian distribution with mean 0 and standard deviation radius/3
    std::normal_distribution<> distZ(0, size.z / 8); // Gaussian distribution with mean 0 and standard deviation radius/3

    for (int i = 0; i < num; ++i) {

        int label = (i % m_nLabel);

        DirectX::XMFLOAT3 pt = m_centers[label];
        pt.x += distX(gen);
        pt.y += distY(gen);
        pt.z += distZ(gen);

        
        m_x.push_back({pt.x,pt.y,pt.z});
        m_y.push_back({ (double)label });
    }


}
