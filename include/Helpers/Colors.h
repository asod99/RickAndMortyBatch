#pragma once
#include <iostream>
#include <string>

namespace Color {
    extern bool useColors;  // Declaración de la variable global

    std::ostream& red(std::ostream &s);
    std::ostream& green(std::ostream &s);
    std::ostream& yellow(std::ostream &s);
    std::ostream& blue(std::ostream &s);
    std::ostream& magenta(std::ostream &s);
    std::ostream& cyan(std::ostream &s);
    std::ostream& white(std::ostream &s);
    std::ostream& reset(std::ostream &s);

    void initialize();
    void printAnimatedText(const std::string& text, std::ostream& (*colorFunc)(std::ostream&), int delay = 10);
}