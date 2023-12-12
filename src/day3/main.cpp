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

#include <list>

#include <ranges>


struct Node
{
    static int sum;

    int digit;// const int i{std::atoi(s)};
    std::string_view::difference_type begin;
    std::string_view::difference_type end;
    bool counted;//= false

    void count() {
        if (!counted) {
            counted = true;
            sum += digit;
        }
    }

    // Node(int digit, )
    //   : begin{ begin_ }, end{ begin_ + static_cast<std::string_view::difference_type>(word_.length()) },
    //     counted{ false } {

    //     // without a digits
    //     if (begin_digit == end_digit.base()) {
    //         counted = true;
    //         digit = 0;
    //     } else {
    //         std::string_view digit_sv{ begin_digit, end_digit.base() };
    //         digit = std::stoi(digit_sv.data());

    //         // contain char in the string
    //         if (digit_sv.size() < word_.size()) { count(); }
    //     }
    // }


    void count(Node &prev) {
        // --- --
        //  ----
        std::string_view::difference_type max_begin = std::max(begin, prev.begin);
        std::string_view::difference_type min_end = std::min(end, prev.end);
        // one charcter distance
        if (max_begin <= min_end) {
            spdlog::debug("{}.{} -> {}.{}", digit, counted, prev.digit, prev.counted);
            count();
            prev.count();
        }
    }
};
int Node::sum = 0;
using List = std::list<Node>;

// TODO: is ugly lose ownership advantage
void seperate_digit(List &list,
  std::string_view::const_iterator begin,
  std::string_view::const_iterator end,
  std::string_view::const_iterator line_begin) {
    auto is_digit = [](char cha) { return static_cast<bool>(std::isdigit(cha)); };
    auto not_digit = [](char cha) { return ! static_cast<bool>(std::isdigit(cha)); };

    bool pass_symbol = false;
    while (begin != end) {
        std::string_view::const_iterator begin_digit = std::find_if(begin, end, is_digit);
        std::string_view::const_iterator begin_symbol = std::find_if(begin, end, not_digit);

        
        if (begin != begin_digit) {
            // symbol
            pass_symbol = true;
            Node node{ .digit = 0, .begin = (begin - line_begin), .end = (begin_digit - line_begin), .counted = true };
            list.push_back(node);
            begin = begin_digit;
        } else {
            // digit
            std::string_view digit{ begin_digit, begin_symbol };
            Node node{ .digit = (std::stoi(digit.data())),
                .begin = (begin_digit - line_begin),
                .end = (begin_symbol - line_begin),
                .counted = false };

            list.push_back(node);

            if (pass_symbol || (begin_digit - begin_symbol) != (begin - end)) {
                spdlog::debug("*{}", node.digit);
                node.count();
            }

            begin = begin_symbol;
        }

    }
}


std::unique_ptr<List> generate_list(const std::string_view line) {
    std::string_view::iterator begin = line.cbegin();
    std::unique_ptr<List> list = std::make_unique<List>();
    auto is_not_dot = [](char cha) { return cha != '.'; };
    // auto is_dot = [](char cha) {return cha != '.';};
    while ((begin = std::find_if(begin, line.cend(), is_not_dot)) != line.cend()) {
        std::string_view::iterator end = std::find(begin, line.cend(), '.');
        // if (end == std::string_view::npos) {end = line.end();}
        seperate_digit(*list, begin, end, line.cbegin());
        begin = end;
    }
    return list;
}

// TODO: benchmark with https://stackoverflow.com/questions/1092561/is-returning-a-stdlist-costly

void count_word(List &current, List &prev) {
    auto current_iter = current.begin();
    auto prev_iter = prev.begin();
    while (current_iter != current.end() && prev_iter != prev.end()) {
        current_iter->count(*prev_iter);

        if (current_iter->end < prev_iter->end) {
            ++current_iter;
        } else {
            ++prev_iter;
        }
    }
}


void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\input.txt" };
    // you can earase the the string each read
    std::string myline;
    int num_line = 0;

    std::unique_ptr<List> prev = std::make_unique<List>();
    while (std::getline(input_file, myline)) {
        spdlog::debug("line {}:\t{}", ++num_line, myline);
        std::unique_ptr<List> current = generate_list(myline);
        count_word(*current, *prev);
        prev = std::move(current);
    }

    fmt::print("sum: {}\n", Node::sum);
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
