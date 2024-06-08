#include "pch.h"
#include "MLMeanSqauredError.h"

using namespace ML;
ML::MeanSqauredError::MeanSqauredError()
    :Loss(LossKind::MeanSqure)
{
}
double MeanSqauredError::Calculate(Vector yp, Vector yt, bool isLogit)
{
    int n = yp.size();

    double ret = 0;
    for (int i = 0; i < yp.size(); ++i)
    {
        ret += pow(yt[i] - yp[i], 2)/2.0;
    }
    return ret;
}


Vector MeanSqauredError::Gradient(Vector yp, Vector yt)
{
    Vector gd;
    
    for (int i = 0; i < yp.size(); ++i)
    {
        gd.push_back(yp[i] - yt[i]);
    }

    return gd;
}