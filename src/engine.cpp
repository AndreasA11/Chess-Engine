#include "engine.hpp"
#include <chrono>


std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> Engine::getTimeInMS() {
    auto start = std::chrono::high_resolution_clock::now();
    return start;
}

int Engine::getTimeInterval(
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> begin, 
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> end) {
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    return duration;
}


