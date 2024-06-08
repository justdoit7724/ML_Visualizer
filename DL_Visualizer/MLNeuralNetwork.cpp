#include "pch.h"
#include "MLNeuralNetwork.h"
#include "MLNormalizer.h"
#include "MLLayer.h"
#include "MLLoss.h"
#include "MLActivation.h"

using namespace ML;

ML::NeuralNetwork::NeuralNetwork()
	:m_normalized(false), m_loss(nullptr), m_epoch(0)
{
}

ML::NeuralNetwork::NeuralNetwork(const NeuralNetwork& network)
{
	Release();

	for (int i = 0; i < network.m_layers.size(); ++i)
	{
		m_layers.push_back(new Layer(network.m_layers[i]->m_nInput, network.m_layers[i]->m_nN, network.m_layers[i]->Act()->m_kind));

		m_layers.back()->m_W =network.m_layers[i]->m_W;
		m_layers.back()->m_B = network.m_layers[i]->m_B;
		
	}

	FactoryLoss::Create(network.m_loss->m_kind, &m_loss);
}

NeuralNetwork::~NeuralNetwork()
{
	Release();
}

void NeuralNetwork::Compile(std::vector<std::pair<ActKind, int>> layers, LossKind loss, Matrix x, Matrix y,int epoch, bool isNormalize)
{
	Release();

	m_epoch = epoch;

	int na = x[0].size();
	int m = x.size();

	FactoryLoss::Create(loss, &m_loss);

	m_layers.push_back(new Layer(na, layers.front().second,(ActKind)(layers.front().first)));
	for (int i=1; i< layers.size();++i)
	{
		ActKind kind = layers[i].first;
		int neuronNum = layers[i].second;

		m_layers.push_back(new Layer(layers[i - 1].second, neuronNum, kind));
	}

	m_y = y;
	if (m_normalized = isNormalize)
	{
		Normalizer::ZNormalize(x, &m_mx, &m_mu, &m_sig);
	}
	else
	{
		m_mx = x;

		m_mu.clear();
		m_sig.clear();
	}

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

}

Vector NeuralNetwork::Train(double alpha)
{
	//shuffle
	for (int i = 0; i < m_mx.size()/100; ++i)
	{
		int i1 = rand() % m_mx.size();
		int i2 = rand() % m_mx.size();

		std::swap(m_mx[i1], m_mx[i2]);
		std::swap(m_y[i1], m_y[i2]);
	}

	Vector hist;
	for (int i = 0; i < m_mx.size(); ++i)
	{
		Matrix A;
		Matrix Z;
		Vector z; 
		A.push_back(m_mx[i]);
		A.push_back(m_layers[0]->Calc(m_mx[i], z));
		Z.push_back(z);

		for (int j = 1; j < m_layers.size(); ++j)
		{
			A.push_back(m_layers[j]->Calc(A[j], z));
			Z.push_back(z);
		}

		//backpropagation
		// 
		//Cost diff
		m_prevDiff = ToMatrix(m_loss->Gradient(A.back(), m_y[i]));

		//layer diff
		for (int j = m_layers.size() - 1; j >= 0; --j)
		{
			Layer* layer = m_layers[j];

			
			Matrix dLZ = layer->Act()->Diff(Z[j]);

			//next diff mat before updating weights
			Matrix dLA = Transpose(Dot(Transpose(dLZ), layer->m_W));
			Matrix nextDiff = Dot(dLA, m_prevDiff);


			//make cur matrix
			Matrix dCZ = Dot(dLZ, m_prevDiff);

			//get gd
			for (int y = 0; y < layer->m_nInput; ++y)
			{
				for (int x = 0; x < layer->m_nN; ++x)
				{
					auto gd = dCZ[x][0] * A[j][y];
					layer->m_W[x][y] -= gd * alpha;
				}
			}
			for (int y = 0; y < layer->m_nN; ++y)
			{
				layer->m_B[y] -= dCZ[y][0] * alpha;
			}

			m_prevDiff= nextDiff;
		}

		if (i % m_epoch == 0)
		{
			int amount = m_mx.size() / m_epoch;
			if (amount > 50)
				amount = 50;
			Matrix pred = Predict(m_mx, 0, amount);
			double cost = 0;
			for (int i = 0; i < pred.size(); ++i)
			{
				cost += m_loss->Calculate(pred[i], m_y[i]);
			}
			cost /= pred.size();
			hist.push_back(cost);
		}
	}


	return hist;
}

Matrix NeuralNetwork::Predict(Matrix x, int start , int size)
{
	if (size < 0 || start < 0)
	{
		start = 0;
		size = x.size();
	}

	Matrix output;

	if (m_normalized)
	{
		for (int i = 0; i < size; ++i)
		{
			x[start+i] = (x[start + i] - m_mu)/m_sig;
		}
	}

	for (int i=0; i< size; ++i)
	{
		Vector v = x[i+start];

		for (Layer* l : m_layers)
		{
			Vector z;
			v = l->Calc(v, z);
		}

		output.push_back(v);
	}

	/*if (m_normalized)
	{
		for (int i = 0; i < output.size(); ++i)
		{
			output[i] = BinaryMul(output[i] , m_sig) + m_mu;
		}
	}*/

	return output;
}

void NeuralNetwork::Release()
{
	m_epoch = 0;
	for (int i = 0; i < m_layers.size(); ++i)
	{
		delete m_layers[i];
	}
	m_layers.clear();

	m_sig.clear();
	m_mu.clear();
	m_normalized = false;

	if (m_loss)
		delete m_loss;

	m_mx.clear();
	m_y.clear();
}

void NeuralNetwork::SetWeights(int l, const Matrix& w, Vector b)
{
	m_layers[l]->m_W = w;
	m_layers[l]->m_B = b;
}

void NeuralNetwork::GetWeights(int l, Matrix& w, Vector& b)
{
	b = m_layers[l]->m_B;
	w = m_layers[l]->m_W;
}

std::vector<std::pair<ActKind, int>> NeuralNetwork::GetLayerInfo()
{
	std::vector <std::pair<ActKind, int >> layer;


	
	for (int i = 0; i < m_layers.size(); ++i)
	{
		layer.push_back({ m_layers[0]->Act()->m_kind,m_layers[i]->m_nN});
	}

	return layer;
}
