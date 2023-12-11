#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <functional>
#include <spdlog/spdlog.h>
#include <tuple>

struct Game
{
    int red;
    int green;
    int blue;

    Game &operator+=(const Game &rhs) {
        red += rhs.red;
        green += rhs.green;
        blue += rhs.blue;
        return *this;
    }

    // TODO: change to spaceship operator
    bool operator<(const Game &rhs) const { return red < rhs.red || green < rhs.green || blue < rhs.blue; }
};

std::tuple<int, std::string_view> get_number(std::string_view line) {
    auto is_digit = [](char charcter) -> bool { return static_cast<bool>(std::isdigit(charcter)); };
    auto is_not_digit = [](char charcter) -> bool { return !static_cast<bool>(std::isdigit(charcter)); };
    const std::string_view::const_iterator first = std::find_if(line.cbegin(), line.cend(), is_digit);
    const std::string_view::const_iterator last = std::find_if(first, line.cend(), is_not_digit);
    line.remove_prefix(static_cast<std::string_view::size_type>(last - line.cbegin()));
    return { std::stoi(std::string_view{ first, last }.data()), line };
}

std::string_view update_color(Game &game, int num, std::string_view line) {
    auto is_alpha = [](char charcter) -> bool { return static_cast<bool>(std::isalpha(charcter)); };
    auto is_not_alpha = [](char charcter) -> bool { return !static_cast<bool>(std::isalpha(charcter)); };
    const std::string_view::const_iterator first = std::find_if(line.cbegin(), line.cend(), is_alpha);
    const std::string_view::const_iterator last = std::find_if(first, line.cend(), is_not_alpha);
    const std::string_view type{ first, last };
    if (type == "red") {
        game.red = num;
    } else if (type == "green") {
        game.green = num;
    } else if (type == "blue") {
        game.blue = num;
    }
    line.remove_prefix(static_cast<std::string_view::size_type>(last - line.cbegin()));
    return line;
}

Game parse_set(std::string_view line) {
    Game res{ 0, 0, 0 };
    while (!line.empty()) {
        auto [num, afte_num_line] = get_number(line);
        // TODO here is an issue maybe end up empty
        line = update_color(res, num, afte_num_line);
    }
    return res;
}

int parse_game(std::string_view line, Game limit) {
    auto [num, after_game] = get_number(line);
    line = after_game;

    std::size_t pos = 0;
    // TODO: post processing, less duplicates
    while ((pos = line.find_first_of(';')) != std::string::npos) {
        const Game set = parse_set({ line.data(), pos });
        line.remove_prefix(std::string_view::size_type{pos + 1});// +1 remove ';'
        if (limit < set) {return 0;}
    }
    const Game last = parse_set(line);
    if (limit < last) {return 0;}
    return num;
}

void read_input() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\input.txt" };
    std::ifstream limit_file{ dir_path + "\\limit.txt" };
    std::string myline;

    std::getline(limit_file, myline);
    const Game limit = parse_set(myline);

    int sum_id = 0;
    while (std::getline(input_file, myline)) {
        sum_id += parse_game(myline, limit);
    }
    fmt::print("num_valid_games: {}\n", sum_id);
}

int main() {
    using namespace std::chrono;
    const time_point start_time{ system_clock::now() };
    try {
        read_input();
    } catch (const std::exception &e) { spdlog::error("what: {}", e.what()); }
    const time_point end_time = std::chrono::system_clock::now();
    fmt::print("Run time: {}us\n", duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    return 0;
}
