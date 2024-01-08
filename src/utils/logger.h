#pragma once

#include <fstream>
#include <iostream>

#define STR(x) #x

class Logger {
  public:
    static void set_logging(bool is_enabled) { enabled = is_enabled; }

    static bool is_enabled() { return enabled; }

    template <typename... Ts>
    static void log(const Ts&... args) {
        if (!enabled) {
            return;
        }

        std::ofstream logfile{filename, std::ios_base::app};
        if (logfile.fail()) {
            std::cerr << "Can't open log file\n";
            return;
        }

        ((logfile << args), ...);
        logfile << "\n";
    }

    template <typename... Ts>
    static void log_with_name(const Ts&... args) {
        if (!enabled) {
            return;
        }

        std::ofstream logfile{filename, std::ios_base::app};
        if (logfile.fail()) {
            std::cerr << "Can't open log file\n";
            return;
        }

        ((logfile << STR(args) << ":" << args << "\t"), ...);
        logfile << "\n";
    }

  private:
    static inline const std::string filename{"../../log/debug_log.txt"};
    static inline bool enabled{true};
};

#ifdef NDEBUG
#define LOG(...) ((void)0)
#else
#define LOG(...) Logger::log("[", __func__, "] ", __VA_ARGS__)
#endif

#define FORCE_LOG(...) Logger::log("[", __func__, "]  ", __VA_ARGS__)