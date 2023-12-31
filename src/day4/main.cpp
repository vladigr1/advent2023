#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <functional>
#include <memory>
#include <spdlog/spdlog.h>
#include <advent2023/sample_library.hpp>

#include <unordered_set>

unsigned get_card_score(std::string_view myline) {
    std::string_view::const_iterator end_winning_num = std::find(myline.cbegin(), myline.cend(), '|');
    std::string_view::const_iterator start_winning_num = std::find(myline.cbegin(), myline.cend(), ':');
    std::string_view winning_number{ ++start_winning_num, end_winning_num };
    spdlog::debug("winning number: {}", winning_number);

    if (end_winning_num == myline.cend()) { return 0; }// no cards
    std::string_view card_number{ ++end_winning_num, myline.cend() };
    spdlog::debug("card number: {}", card_number);
    // TODO: implment score
    std::unordered_set<int> winning_set;
    auto update_set = [&winning_set](int num) { winning_set.insert(num); };

    apply_str_num(winning_number, update_set);

    unsigned match = 0;
    auto update_match = [&match, &winning_set](int num) {
        if (winning_set.contains(num)) {
            spdlog::debug("match number: {}", num);
            ++match;
        }
    };

    apply_str_num(card_number, update_match);

    spdlog::debug("num matches: {}", match);

    return static_cast<unsigned>(std::exp2(static_cast<double>(match - 1)));
}

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\input.txt" };
    // you can earase the the string each read
    std::string myline;
    int num_line = 0;

    unsigned score = 0;
    while (std::getline(input_file, myline)) {
        spdlog::debug("line {}:\t{}", ++num_line, myline);
        score += get_card_score(myline);
    }

    fmt::print("Score: {}\n", score);
}


int main() {
    using namespace std::chrono;
    // spdlog::set_level(spdlog::level::debug);// Set global log level to debug
    // spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    const time_point<steady_clock> start_time = steady_clock::now();
    try {
        solver();
    } catch (const std::exception &e) { spdlog::error("what: {}", e.what()); }
    const time_point<steady_clock> end_time = std::chrono::steady_clock::now();
    fmt::print("Run time: {}us\n", duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    return 0;
}
