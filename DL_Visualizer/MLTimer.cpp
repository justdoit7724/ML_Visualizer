#include "pch.h"
#include "MLTimer.h"

using namespace ML;

void Timer::Start()
{
    start_time = std::chrono::high_resolution_clock::now();
}

double Timer::GetElapsed()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_seconds = current_time - start_time;
    return elapsed_seconds.count();
}
