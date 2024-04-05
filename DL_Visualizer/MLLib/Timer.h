#pragma once
#include <chrono>

namespace ML
{
	class Timer
	{
	public:

		void Start();
		double GetElapsed();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
	};
}

