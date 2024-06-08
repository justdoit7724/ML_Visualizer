#include "pch.h"
#include "MLFactoryAct.h"
#include "MLLinearAct.h"
#include "MLLogisticAct.h"
#include "MLSoftmaxAct.h"
#include "MLReluAct.h"

using namespace ML;

void FactoryAct::Create(ActKind kind, Activation** out)
{
	switch (kind)
	{
	case ActKind::Linear:
		*out = new LinearAct();
		break;
	case ActKind::Logistic:
		*out = new LogisticAct();
		break;
	case ActKind::Relu:
		*out = new ReluAct();
		break;
	case ActKind::Softmax:
		*out = new SoftmaxAct();
		break;
	default:
		break;
	}
}
