#include <fmt/printf.h>
#include <spdlog/spdlog.h>

#include <fmt/color.h>
#include <iostream>

int main() {
    try {
        std::cout << fmt::format(fg(fmt::color::crimson) | fmt::emphasis::bold, "Hello, {}!\n", "world") << std::endl;

        std::cout << fmt::format(
          fg(fmt::color::floral_white) | bg(fmt::color::slate_gray) | fmt::emphasis::underline, "Hello, {}!\n", "???")
                  << std::flush;

        std::cout << fmt::format(fg(fmt::color::steel_blue) | fmt::emphasis::italic, "Hello, {}!\n", "??") << std::endl;
        fmt::printf("hello world");
        std::string a;
        std::cin >> a;
    } catch (const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
    return 0;
}
