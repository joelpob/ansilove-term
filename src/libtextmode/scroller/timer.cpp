#include "timer.h"
#include <chrono>

timer_t::timer_t()
{
    start = std::chrono::system_clock::now();
}

double timer_t::check()
{
    now = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration = now - start;
    return duration.count();
}

void timer_t::reset()
{
    start = now;
}
