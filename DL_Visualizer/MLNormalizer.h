#pragma once
#include "MLMathHelp.h"
namespace ML
{
	class Normalizer
	{
	public:
		static void ZNormalize(const Matrix& x, Matrix* outX, Vector* outMu, Vector* outSig);
	};

}