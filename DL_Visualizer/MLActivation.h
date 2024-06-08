#pragma once
#include "MLMathHelp.h"
#include "MLFactoryAct.h"

namespace ML
{
	class Activation
	{
	public:
		Activation() = delete;
		Activation(ActKind kind);
		const ActKind m_kind;

		virtual Vector Calc(Vector z) = 0;
		virtual Matrix Diff(Vector z) = 0;

	};
}

