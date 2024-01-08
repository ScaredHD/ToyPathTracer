#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

class Timer {
  public:
    using MilliSeconds = std::chrono::milliseconds;

    Timer() { start = Clock::now(); }

    size_t reset() {
        auto now      = Clock::now();
        auto duration = std::chrono::duration_cast<MilliSeconds>(now - start);
        start         = now;
        return duration.count();
    }

    size_t duration() const {
        auto now      = Clock::now();
        auto duration = std::chrono::duration_cast<MilliSeconds>(now - start);
        return duration.count();
    }

  private:
    using Clock = std::chrono::system_clock;
    std::chrono::time_point<std::chrono::system_clock> start;
};

inline double to_seconds(size_t milliseconds) {
    return static_cast<double>(milliseconds) / 1000;
}

inline double to_minutes(size_t milliseconds) {
    return static_cast<double>(milliseconds) / 60000.0;
}

inline std::string format_time(size_t milliseconds) {
    double value{};
    std::string suffix{};

    auto ms = static_cast<double>(milliseconds);
    if (ms < 1000.0) {
        value  = ms;
        suffix = "ms";
    } else if (ms < 60000.0) {
        value  = to_seconds(milliseconds);
        suffix = "s";
    } else {
        value  = to_minutes(milliseconds);
        suffix = "min";
    }
    return std::to_string(value) + suffix;
}
