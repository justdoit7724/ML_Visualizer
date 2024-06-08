#pragma once
#include "DXGraphic.h"

namespace DX
{
    class  WorldGraphic :
        public Graphic
    {
    public:
        WorldGraphic(HWND hwnd, int msaa = 1);

    private:
    };
}
