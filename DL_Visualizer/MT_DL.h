#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include "MLLib/NeuralNetwork.h"
#include "InputPoints.h"

class MT_DL
{
public:
	MT_DL();
	~MT_DL();

	void Initialize(std::vector<std::pair<ML::ActKind, int>> layer, const InputPoints* input, ML::LossKind loss, double alpha, int epoch);
	void EnableTraining(bool enable);
	std::vector<double> GetHist();
	ML::Matrix GetPredict(ML::Matrix x);

private:

	void Run();


	std::mutex m_mutexTrain;

	std::thread* m_thread;
	ML::NeuralNetwork* m_RNN;
	ML::NeuralNetwork* m_RNNPred;

	std::queue<double> m_hist;

	ML::Matrix m_input;
	ML::Matrix m_pred;

	int m_epoch;
	double m_alpha;

	bool m_run;
	bool m_train;
};

