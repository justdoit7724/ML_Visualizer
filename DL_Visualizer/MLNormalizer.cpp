#include "pch.h"
#include "MLNormalizer.h"
using namespace ML;
void Normalizer::ZNormalize(const Matrix& x, Matrix* outX, Vector* outMu, Vector* outSig)
{
	int n = x[0].size();
	int m = x.size();

	outMu->resize(n);
	outSig->resize(n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			(*outMu)[i] += x[j][i];
		}
		(*outMu)[i] /= m;
	}

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			(*outSig)[i] += pow(x[j][i] - (*outMu)[i], 2);
		}
		(*outSig)[i] = sqrt((*outSig)[i]);
	}
	
	outX->clear();
	outX->resize(m, Vector(n, 0));

	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			(*outX)[i][j] = (x[i][j] - (*outMu)[j]) / (*outSig)[j];
		}
	}
}