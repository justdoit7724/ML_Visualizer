#pragma once
#include "MLActivation.h"

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
