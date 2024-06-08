#pragma once
#include "MLLoss.h"
namespace ML {
    class MeanSqauredError :
        public Loss
    {
    public:
        MeanSqauredError();
        double Calculate(Vector yp, Vector yt, bool isLogit = false) override;
        Vector Gradient(Vector yp, Vector yt) override;
    };
}

