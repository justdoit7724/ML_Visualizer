#pragma once
#include "InputPoints.h"

class IP_Radius :
    public InputPoints
{
public:
    IP_Radius(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 size, int num);
};

