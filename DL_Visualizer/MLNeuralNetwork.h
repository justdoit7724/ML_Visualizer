#pragma once
#include "MLMathHelp.h"
#include "MLFactoryAct.h"
#include "MLFactoryLoss.h"

namespace ML
{
	class Layer;
	class Loss;
	class NeuralNetwork
	{
	public:

		NeuralNetwork();
		NeuralNetwork(const NeuralNetwork& network);
		virtual ~NeuralNetwork();

		virtual void Compile(std::vector<std::pair<ActKind,int>> layers, LossKind loss, Matrix x, Matrix y,int epoch, bool isNormalize);
		virtual Vector Train(double alpha);

		Matrix Predict(Matrix x, int start=-1, int size=-1);
		void Release();

		void SetWeights(int l, const Matrix& w, Vector b);
		void GetWeights(int l, Matrix& w, Vector& b);

		std::vector<std::pair<ActKind, int>> GetLayerInfo();

	protected:

		std::vector<Layer*> m_layers;
		Loss* m_loss;

		bool m_normalized;
		Matrix m_mx;
		Matrix m_y;
		Vector m_mu;
		Vector m_sig;
		int m_epoch;

		Matrix m_prevDiff;
	};
}
