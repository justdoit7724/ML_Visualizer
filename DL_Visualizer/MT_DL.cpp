#include "pch.h"
#include "MT_DL.h"

MT_DL::MT_DL()
	:m_run(true), m_train(false), m_RNNPred(nullptr)
{
	m_thread = new std::thread(&MT_DL::Run, this);

	m_RNN = new ML::NeuralNetwork();
}

MT_DL::~MT_DL()
{
	m_run = false;
	m_thread->join();
	delete m_thread;
}

void MT_DL::Initialize(std::vector<std::pair<ML::ActKind, int>> layer, const InputPoints* input, ML::LossKind loss, double alpha, int epoch)
{
	m_alpha = alpha;
	m_epoch = epoch;

	m_input = input->GetData();
	auto y = input->GetLabel(layer.back().second > 1);

	m_RNN->Compile(layer, loss, m_input, y,epoch, false);

	if (m_RNNPred)
		delete m_RNNPred;
	m_RNNPred = new ML::NeuralNetwork(*m_RNN);
}

void MT_DL::EnableTraining(bool enable)
{
	m_train = enable;
}

void MT_DL::Run()
{
	while (m_run)
	{
		while (m_train)
		{
			std::vector<double> hist;
			hist = m_RNN->Train(m_alpha);
			{
				std::unique_lock<std::mutex> lock(m_mutexTrain);
				ML::Matrix w;
				ML::Vector b;
				auto layer = m_RNN->GetLayerInfo();
				for (int i = 0; i < layer.size(); ++i)
				{
					m_RNN->GetWeights(i, w, b);
					m_RNNPred->SetWeights(i, w, b);
				}
			
				for (int i = 0; i < hist.size(); ++i)
					m_hist.push(hist[i]);
			}
		}
	}
}

std::vector<double> MT_DL::GetHist()
{
	const std::unique_lock<std::mutex> lock(m_mutexTrain);

	std::vector<double> output;
	int size = m_hist.size();
	for (int i = 0; i < size; ++i)
	{
		output.push_back(m_hist.front());
		m_hist.pop();
	}

	return output;
}

ML::Matrix MT_DL::GetPredict(ML::Matrix x)
{
	const std::unique_lock<std::mutex> lock(m_mutexTrain);

	return m_RNNPred->Predict(x);
}
