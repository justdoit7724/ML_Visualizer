#include "pch.h"
#include "MLSoftmaxAct.h"

using namespace ML;

SoftmaxAct::SoftmaxAct()
	:Activation(ActKind::Softmax)
{
}

Vector SoftmaxAct::Calc(Vector z)
{
	Vector output = Exp(z);
	double sum = std::accumulate(output.begin(), output.end(),0.0);
	output /= sum;

	return output;
}

Matrix SoftmaxAct::Diff(Vector z)
{
	int n = z.size();

	Matrix output(n, Vector(n,0));

	Vector expZ = Exp(z);
	double sum = std::accumulate(expZ.begin(), expZ.end(), 0.0);

	for (int y = 0; y < n; ++y)
	{
		for (int x = 0; x < n; ++x)
		{
			if (x == y)
			{
				output[y][x] += (expZ[y] * sum - pow(expZ[y], 2));
			}
			else
			{
				output[y][x] +=  - expZ[x] * expZ[y];
			}

		}
	}

	output /= (sum * sum);

	return output;
}
