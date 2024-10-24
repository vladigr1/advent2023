#include <chrono>
#include <fstream>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <spdlog/fmt/ranges.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <numeric>
#include <ranges>

#include <map>

struct Serie
{
  public:
    using Map = std::map<std::pair<int, int>, long long int>;
    Map serie;
    int num_column;
    int zero_row;

  private:
    Map::value_type::second_type calc_value(int row, int column) {
        long long int value;
        auto iter = serie.find({ row, column });
        if (iter != serie.end()) {
            value = iter->second;
        } else {
            value = calc_value(row - 1, column + 1) - calc_value(row - 1, column);
        }
        spdlog::trace("calc({},{})={}", row, column, value);
        serie.emplace(std::pair{ row, column }, value);
        return value;
    }

    bool validate_all_zero_row(int row) {
        spdlog::debug("validate_all_zero_row({})", row);
        int end = static_cast<int>(num_column - row);
        if (end < 1) { return true; }
        for (const int column : std::views::iota(0, end)) {
            if (calc_value(row, column) != 0) { return false; }
        }
        return true;
    }

  public:
    Serie(std::string &line) {
        std::istringstream iss(line);
        int column{ 0 };
        while (!iss.eof()) {
            int value;
            iss >> value;
            serie.emplace(std::pair{ 0, column }, value);
            ++column;
        }
        num_column = column;
        spdlog::debug("serie={}\n", serie);
        spdlog::debug("num_column={}\n", num_column);

        for (const int row : std::views::iota(1, num_column)) {
            if (validate_all_zero_row(row)) {
                zero_row = row;
                return;
            }
        }
        spdlog::debug("zero_row={}", zero_row);
    }

    long long int get_next_value() {
        long long int value = 0;
        for (const int row : std::views::iota(0, zero_row)) {
            long long int factor = serie.at({ row, 0 });
            long long int multiplier = 1;
            long long int divder = 1;
            for (int i : std::views::iota(1, row+1)) {
                multiplier *= (num_column - i + 1);
                divder *= i;
            }
            spdlog::debug("row={} factor={} multiplier={} divder={}\n", row, factor, multiplier, divder);
            value += factor * multiplier / divder;
            if(multiplier % divder != 0){
                spdlog::error("bug must divide evenly");
            }
        }
        spdlog::debug("get_next_value={}", value);
        return value;
    }
};

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input{ dir_path + "\\input.txt" };

    std::string line;
    long long int sum = 0;
    while (std::getline(input, line)) {

        Serie serie(line);
        sum += static_cast<double>(serie.get_next_value());
        
    }
    fmt::print("Total sum = {}\n", sum);
}

int main() {
    using namespace std::chrono;
    spdlog::set_level(spdlog::level::debug);// Set global log level to debug
    // auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    // spdlog::set_default_logger(logger);
    // spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    const time_point<steady_clock> start_time = steady_clock::now();
    try {
        solver();
    } catch (const std::exception &e) { spdlog::error("what: {}", e.what()); }
    const time_point<steady_clock> end_time = std::chrono::steady_clock::now();
    fmt::print("Run time: {}us\n", duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    return 0;
}
