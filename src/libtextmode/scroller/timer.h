#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class timer_t
{
private:
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> now;
public:
    timer_t();
    double check();
    void reset();
};

#endif
