#pragma once
#include "DXGraphic.h"

namespace DX
{
	class Component
	{

	public:
		Component();
	protected:

		virtual void Apply(const Graphic* graphic) = 0;
	};
}

