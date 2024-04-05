#pragma once
#include "MathHelp.h"
#include "FactoryAct.h"

namespace ML
{
	class Activation;

	class Layer
	{
	public:
		Layer(int nInput, int nN, ActKind act);

		Vector Calc(Vector a, Vector& z);
		Activation* Act();
		
		const int m_nInput;
		const int m_nN;
		Matrix m_W;
		Vector m_B;

	private:

		Activation* m_act;
	};
}
