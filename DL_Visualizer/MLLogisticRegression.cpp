#include "pch.h"
#include "MLLogisticRegression.h"
#include "MLFactoryLoss.h"


using namespace ML;
LogisticRegression::LogisticRegression()
{
	FactoryLoss::Create(LossKind::BinaryCrossEntropy, &m_loss);
}
Vector LogisticRegression::Func(Matrix& x, Vector& w, double b)
{
	int m = x.size();
	int n = x[0].size();

	Vector ret(m);
	for (int i = 0; i < m; ++i)
	{
		double z = b;
		for (int j = 0; j < n; ++j)
		{
			z += x[i][j] * w[j];
		}

		ret[i] = 1.0 / (1.0 + exp(-z));
	}
	return ret;
}
double LogisticRegression::Cost(Matrix& x, Vector& y, Vector& w, double b) {

	Vector yPred = Func(x, w, b);

	return m_loss->Calculate(yPred, y);
}
