// Colors.h
#pragma once
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Color {
    bool useColors = false;

    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";

    inline std::ostream& red(std::ostream &s) {
        return useColors ? s << RED : s;
    }

    inline std::ostream& green(std::ostream &s) {
        return useColors ? s << GREEN : s;
    }

    inline std::ostream& yellow(std::ostream &s) {
        return useColors ? s << YELLOW : s;
    }

    inline std::ostream& blue(std::ostream &s) {
        return useColors ? s << BLUE : s;
    }

    inline std::ostream& magenta(std::ostream &s) {
        return useColors ? s << MAGENTA : s;
    }

    inline std::ostream& cyan(std::ostream &s) {
        return useColors ? s << CYAN : s;
    }

    inline std::ostream& white(std::ostream &s) {
        return useColors ? s << WHITE : s;
    }

    inline std::ostream& reset(std::ostream &s) {
        return useColors ? s << RESET : s;
    }

    void initialize() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (SetConsoleMode(hOut, dwMode)) {
            useColors = true;
        }
#else
        if (isatty(fileno(stdout))) {
            useColors = true;
        }
#endif
    }

    void printAnimatedText(const std::string& text, std::ostream& (*colorFunc)(std::ostream&), int delay = 50) {
        for (char c : text) {
            std::cout << colorFunc << c << reset;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        std::cout << std::endl;
    }
}