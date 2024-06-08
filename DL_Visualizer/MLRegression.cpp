#include "pch.h"
#include "MLRegression.h"
#include "MLLoss.h"
#include "MLNormalizer.h"

using namespace ML;



Regression::Regression(const Regression& reg)
{
	m_w = reg.m_w;
	m_b = reg.m_b;
	m_isNormalized = reg.m_isNormalized;
	m_mu = reg.m_mu;
	m_sig = reg.m_sig;

	m_nVar = reg.m_nVar;

	FactoryLoss::Create(reg.m_loss->m_kind, &m_loss);
}

Regression::~Regression()
{
	delete m_loss;
}

bool Regression::Compile(Matrix&  x, Vector& y,int epoch, bool isNormalize)
{
	m_nVar = x[0].size();
	m_w.resize(m_nVar);
	for (int i = 0; i < m_nVar; ++i)
	{
		m_w[i] = 2 * (rand() / (double)RAND_MAX - 0.5);
	}
	m_b = 2*(rand()/ (double)RAND_MAX-0.5);
	m_mx.resize(x.size(), Vector(m_nVar, 0));

	m_isNormalized = isNormalize;
	if (m_isNormalized)
	{
		Normalizer::ZNormalize(x, &m_mx, &m_mu, &m_sig);
	}
	else
		m_mx = x;

	m_y = y;

	int m = x.size();
	m_mx.reserve(m * epoch);
	m_y.reserve(m * epoch);
	for (int i = 0; i < (epoch - 1); ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			m_mx.push_back(m_mx[j]);
			m_y.push_back(m_y[j]);
		}
	}

	return false;
}

Vector Regression::Train(double alpha)
{
	Vector hCost;


	//shuffle
	/*for (int i = 0; i < m_mx.size() / 2; ++i)
	{
		int i1 = rand() % m_mx.size();
		int i2 = rand() % m_mx.size();

		std::swap(m_mx[i1], m_mx[i2]);
		std::swap(m_y[i1], m_y[i2]);
	}*/


	double cost = Cost(m_mx, m_y, m_w, m_b);
	hCost.push_back(cost);
	Vector gdw;
	double gdb;
	Gradient(m_mx, m_y, m_w, m_b,gdw,gdb);

	m_w-=gdw * alpha;
	m_b -= gdb* alpha;

	return hCost;
}

Vector Regression::Predict(Matrix&  x)
{
	Vector ret;
	Matrix mx = m_isNormalized? ZNormalize(x): x;

	return Func(mx, m_w, m_b);
}

void Regression::SetWeights(Vector& w, double b)
{
	m_w = w;
	m_b = b;
}

void Regression::GetWeights(Vector& w, double& b)
{
	w = m_w;
	b = m_b;
}

Matrix ML::Regression::ZNormalize(const Matrix& x)
{
	int n = x[0].size();
	int m = x.size();

	Matrix ret;
	ret.resize(m, Vector(n, 0));

	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			ret[i][j] = (x[i][j] - m_mu[j]) / m_sig[j];
		}
	}

	return ret;
}



void Regression::Gradient(Matrix& x, Vector& y, Vector& w, double b, Vector& gdw, double& gdb)
{
	gdw.clear();
	gdb = 0;

	Vector yPred = Func(x, w, b);
	int m = x.size();
	int n = x[0].size();

	gdw.resize(n, 0);


	for (int j = 0; j < m; ++j)
	{
		double value = m_loss->Gradient({ yPred[j] }, { y[j] })[0];
		for (int i = 0; i < n; ++i)
		{
			gdw[i] += value * x[j][i];
		}
		gdb += value;
	}
	gdw/= m;
	gdb /= m;
}