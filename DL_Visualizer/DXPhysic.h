#pragma once

#include "DXGeometrics.h"

namespace DX {
class Object;

	class Physic
	{
	public:
		static Object* Raycast(const std::vector<Object*>& objs, Geometrics::Ray ray, int mask);
	};
}

