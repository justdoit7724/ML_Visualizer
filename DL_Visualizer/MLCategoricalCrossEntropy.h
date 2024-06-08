#pragma once
#include "MLLoss.h"
namespace ML {
    class CategoricalCrossEntropy :
        public Loss
    {
    public:
        CategoricalCrossEntropy();
        double Calculate(Vector yp, Vector yt, bool isLogit = false) override;
        Vector Gradient(Vector yp, Vector yt) override;
    };

}
