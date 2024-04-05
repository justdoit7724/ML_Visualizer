#pragma once
#include "Activation.h"
namespace ML
{
    class LogisticAct :
        public Activation
    {
    public:
        LogisticAct();
        Vector Calc(Vector z) override;
        Matrix Diff(Vector z) override;
    };
}

