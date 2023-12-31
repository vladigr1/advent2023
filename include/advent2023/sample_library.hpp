#ifndef SAMPLE_LIBRARY_HPP
#define SAMPLE_LIBRARY_HPP

#include <advent2023/sample_library_export.hpp>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <string>

[[nodiscard]] SAMPLE_LIBRARY_EXPORT int factorial(int) noexcept;

[[nodiscard]] constexpr int factorial_constexpr(int input) noexcept {
    if (input == 0) { return 1; }

    return input * factorial_constexpr(input - 1);
}

template<typename Func> void apply_str_num(std::string_view str_num, Func func) {
    auto is_digit = [](char cha) { return static_cast<bool>(std::isdigit(cha)); };
    auto is_not_digit = [](char charcter) -> bool { return !static_cast<bool>(std::isdigit(charcter)); };
    std::string_view::const_iterator first;
    std::string_view::const_iterator last = str_num.cbegin();// first step
    while ((first = std::find_if(last, str_num.cend(), is_digit)) != str_num.cend()) {
        last = std::find_if(first, str_num.cend(), is_not_digit);
        func(std::stoi(std::string_view{ first, last }.data()));
    }
}

#endif
