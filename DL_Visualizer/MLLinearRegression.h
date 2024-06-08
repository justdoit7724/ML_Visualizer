#pragma once
#include "MLRegression.h"
namespace ML {
    class LinearRegression :
        public Regression
    {
    public:
        LinearRegression();

        Vector Func(Matrix&  x, Vector& w, double b) override;
        double Cost(Matrix&  x, Vector& y, Vector& w, double b) override;
    };
}

