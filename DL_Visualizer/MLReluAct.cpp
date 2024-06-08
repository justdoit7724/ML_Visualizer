#include "pch.h"
#include "MLReluAct.h"

using namespace ML;

ML::ReluAct::ReluAct()
	:Activation(ActKind::Relu)
{
}

Vector ReluAct::Calc(Vector z)
{
	Vector output(z.size());

	for (int i = 0; i < output.size(); ++i)
	{
		output[i] = z[i] > 0?z[i] : 0;

	}

	return output;
}

Matrix ReluAct::Diff(Vector z)
{
	int n = z.size();

	Matrix output(n, Vector(n,0));

	for (int i = 0; i < n; ++i)
	{
		output[i][i] = z[i] > 0 ? 1 : 0;
	}

	return output;
}
