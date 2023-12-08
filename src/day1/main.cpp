#include <fstream>
#include <iostream>
#include <string>

#include <algorithm>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <functional>
#include <spdlog/spdlog.h>
#include <chrono>

void sum_code_line(){
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input_file{ dir_path + "\\input.txt" };
    std::string myline;

    if (!input_file.is_open()) {
        spdlog::error("failed to open input");
        return;
    }

    int sum = 0;
    while (std::getline(input_file, myline)) {
        // TODO: check performance with deferent solution
        // size_t last_index = str.find_last_not_of("0123456789");

        auto is_digit = [](char charcter) -> bool { return static_cast<bool>(std::isdigit(charcter)); };
        auto first = std::find_if(myline.cbegin(), myline.cend(), is_digit);
        auto last = std::find_if(myline.rbegin(), myline.rend(), is_digit);
        sum += std::stoi(fmt::format("{}{}\n", *first, *last));
    }
    fmt::print("sum: {}\n", sum);
}

int main() {
    using namespace std::chrono;
    const time_point start_time {system_clock::now()};
    try
    {
        sum_code_line();
    }
    catch(const std::exception& e)
    {
        spdlog::error("what: {}", e.what());
    }
    const time_point end_time = std::chrono::system_clock::now();
    fmt::print("Run time: {}us\n", duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    return 0;
}
