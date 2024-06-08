#pragma once
#include "DXMesh.h"

namespace DX
{
    class  CircleMesh :
        public Mesh
    {
    public:
        CircleMesh(ID3D11Device* device, int sCount);
    };
}
