#pragma once
#include "InputPoints.h"
class IP_Simple :
    public InputPoints
{
public:
    IP_Simple(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int num, int nLabel);

private:
    std::vector<DirectX::XMFLOAT3> m_centers;
};

