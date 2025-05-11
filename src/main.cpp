#include <iostream>
#include <memory>
#include "core/test.cpp"
#include <fmt/core.h>
#include <fmt/color.h>


int main() {
    fmt::print("Hello, {}!\n", "fmt");
    fmt::print(fmt::emphasis::bold | fg(fmt::color::spring_green),
               "Formatted number: {:08}\n", 42);

    return 0;
}


