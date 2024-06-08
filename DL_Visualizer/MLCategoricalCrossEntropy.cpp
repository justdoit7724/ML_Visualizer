#include "pch.h"
#include "MLCategoricalCrossEntropy.h"

using namespace ML;
ML::CategoricalCrossEntropy::CategoricalCrossEntropy()
	:Loss(LossKind::CategoryCrossEntropy)
{
}
double CategoricalCrossEntropy::Calculate(Vector yp, Vector yt, bool isLogit)
{
	auto& my = yp;

	if (isLogit)
	{
		my = Sigmoid(yp);
	}

	double ret = 0;
	int n = yp.size();
	for (int i = 0; i < n; ++i)
	{
		ret += -yt[i] * log(my[i]);
	}

	return ret;
}

Vector CategoricalCrossEntropy::Gradient(Vector yp, Vector yt)
{
	int n = yp.size();
	Vector gd(n,0);
	for (int i = 0; i < n; ++i)
	{
		gd[i] = -yt[i] / yp[i];
	}

	return gd;
}