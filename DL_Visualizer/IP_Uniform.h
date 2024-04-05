#pragma once
#include "InputPoints.h"
class IP_Uniform :
    public InputPoints
{
public:
    IP_Uniform(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int nHor, int nVer);
};

