#pragma once

namespace ML
{
	class Activation;
	enum class ActKind {
		Linear,
		Logistic,
		Relu,
		Softmax
	};

	class FactoryAct
	{
	public:
		static void Create(ActKind kind, Activation** out);
	};

}