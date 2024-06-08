#include "pch.h"
#include "MLLayer.h"
#include "MLActivation.h"

using namespace ML;

Layer::Layer(int nInput, int nN, ActKind act)
	:m_nInput(nInput),m_nN(nN)
{
	m_W.resize(nN, Vector(nInput));
	for (int i = 0; i < nN; ++i)
	{
		for (int j = 0; j < nInput; ++j)
		{
			m_W[i][j] = 2*(rand() / (float)RAND_MAX-0.5);
		}
	}
	m_B.resize(nN);
	for (int i = 0; i < nN; ++i)
	{
		m_B[i] = 2 * (rand() / (float)RAND_MAX - 0.5);
	}

	FactoryAct::Create(act, &m_act);
}

Vector Layer::Calc(Vector a, Vector& z)
{
	z = Transpose(Dot(m_W, ToMatrix(a)))[0] + m_B;

	return m_act->Calc(z);
}

Activation* ML::Layer::Act()
{
	return m_act;
}
