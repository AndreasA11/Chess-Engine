#include <chrono>


namespace Engine {
    //get time (called once to start a timer, called again to end a timer)
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration> getTimeInMS();
    //gets the time inbetween two calls of getTimeInMS();
    int getTimeInterval(std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration>, std::chrono::time_point<std::chrono::steady_clock, std::chrono::steady_clock::duration>);

    int evaluate();
};