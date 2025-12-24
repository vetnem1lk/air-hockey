#include "Time.h"

void Time::update() {
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = now - m_last;
    m_delta = diff.count();
    m_last = now;
}
