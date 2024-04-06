#include <chrono>

// Class for messuring time
class Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> stop;
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    template <typename T>
    long GetTime() {
        stop = std::chrono::high_resolution_clock::now();
        T duration = std::chrono::duration_cast<T>(stop - start);
        return duration.count();
    }
};
