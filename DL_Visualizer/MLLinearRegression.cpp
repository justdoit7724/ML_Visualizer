#include "pch.h"
#include "MLLinearRegression.h"
#include "MLFactoryLoss.h"

using namespace ML;
LinearRegression::LinearRegression()
{
	FactoryLoss::Create(LossKind::MeanSqure, &m_loss);
}
Vector LinearRegression::Func(Matrix&  x, Vector& w, double b)
{
	int m = x.size();
	int n = x[0].size();

	Vector ret(m);
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			ret[i] += x[i][j] * w[j];
		}
		ret[i] += b;
	}

	return ret;
}

double LinearRegression::Cost(Matrix&  x, Vector& y, Vector& w, double b)
{
	Vector yPred = Func(x, w, b);

	return m_loss->Calculate(yPred, y, true);
}
