#pragma once
#include "Regression.h"

namespace ML
{
    class LogisticRegression :
        public Regression
    {
    public:
        LogisticRegression();


        Vector Func(Matrix& x, Vector& w, double b) override;
        double Cost(Matrix& x, Vector& y, Vector& w, double b) override;
    };
}
