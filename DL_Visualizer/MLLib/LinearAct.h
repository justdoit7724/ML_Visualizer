#pragma once
#include "Activation.h"

namespace ML
{
    class LinearAct :
        public Activation
    {
    public:
        LinearAct();

        Vector Calc(Vector z) override;

        Matrix Diff(Vector z) override;


    };
}
