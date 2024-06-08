#pragma once
#include "MLMathHelp.h"
#include "MLFactoryLoss.h"

namespace ML {
	class Loss
	{

	public:

		Loss() = delete;
		Loss(LossKind kind);

		const LossKind m_kind;

		/***
		Single data calculation
		***/
		virtual double Calculate(Vector yp, Vector yt, bool isLogit = false)=0;

		/***
		Single data calculation
		***/
		virtual Vector Gradient(Vector yp, Vector yt) = 0;
	};
}
