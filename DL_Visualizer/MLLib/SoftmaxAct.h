#pragma once
#include "Activation.h"
namespace ML
{
    class SoftmaxAct :
        public Activation
    {
    public:
        SoftmaxAct();

        Vector Calc(Vector z) override;
        Matrix Diff(Vector z) override;
    };
}

