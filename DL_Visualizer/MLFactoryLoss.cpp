#include "pch.h"
#include "MLFactoryLoss.h"
#include "MLBinaryCrossEntropy.h"
#include "MLCategoricalCrossEntropy.h"
#include "MLMeanSqauredError.h"
using namespace ML;

void FactoryLoss::Create(LossKind kind, Loss** out)
{
	switch (kind)
	{
	case ML::LossKind::BinaryCrossEntropy:
		*out = new BinaryCrossEntropy();
		break;
	case ML::LossKind::CategoryCrossEntropy:
		*out = new CategoricalCrossEntropy();
		break;
	case ML::LossKind::MeanSqure:
		*out = new MeanSqauredError();
		break;
	default:
		break;
	}
}
