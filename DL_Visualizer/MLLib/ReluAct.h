#pragma once
#include "Activation.h"
namespace ML
{
    class ReluAct :
        public Activation
    {
    public:
        ReluAct();
       Vector Calc(Vector z) override;
       Matrix Diff(Vector z) override;
    };
}

