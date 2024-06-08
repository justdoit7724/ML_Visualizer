#pragma once
#include "MLLoss.h"

namespace ML {
    class BinaryCrossEntropy :
        public Loss
    {
    public:

        BinaryCrossEntropy();

        double Calculate(Vector yp, Vector yt, bool isLogit = false) override;
        Vector Gradient(Vector yp, Vector yt) override;
    };
}

