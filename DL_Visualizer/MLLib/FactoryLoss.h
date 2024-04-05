#pragma once

namespace ML
{
	class Loss;
	enum class LossKind
	{
		BinaryCrossEntropy,
		CategoryCrossEntropy,
		MeanSqure
	};

	class FactoryLoss
	{
	public:
		static void Create(LossKind kind, Loss** out);
	};
}
