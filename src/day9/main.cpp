#include <chrono>
#include <fstream>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <spdlog/fmt/ranges.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <numeric>
#include <ranges>
#include <sstream>

#include <map>

struct Serie
{
  public:
    using Map = std::map<std::pair<int, int>, double>;
    Map serie;
    int num_column;
    int zero_row;

  private:
    // NOLINTNEXTLINE(misc-no-recursion)
    Map::value_type::second_type calc_value(int row, int column) {
        double value{ 0.0 };
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

    void calc_row(int row) {
        spdlog::debug("calc_row({})", row);
        int end = num_column - row;
        if (end < 1) { return; }
        for (int column = 0; column < end; ++column) { calc_value(row, column); }
    }

    [[nodiscard]] bool validate_all_zero_row(int row) const {
        spdlog::debug("validate_all_zero_row({})", row);
        if (num_column <= row) { return true; }
        int end = num_column - row;

        // NOLINTNEXTLINE(readability-use-anyofallof)
        for (int column = 0; column < end; ++column) {
            if (serie.at(std::pair{ row, column }) != 0) { return false; }
        }
        return true;
    }

  public:
    explicit Serie(std::string &line) {
        std::istringstream iss(line);
        int column{ 0 };
        while (!iss.eof()) {
            int value{ 0 };
            iss >> value;
            serie.emplace(std::pair{ 0, column }, value);
            ++column;
        }
        num_column = column;
        spdlog::debug("serie={}\n", serie);
        spdlog::debug("num_column={}\n", num_column);

        for (int row = 1; row < num_column; ++row) {
            calc_row(row);
            if (validate_all_zero_row(row)) {
                zero_row = row;
                return;
            }
        }
        spdlog::debug("zero_row={}", zero_row);
    }

    double get_next_value() {
        double value = 0;
        for (int row = 0; row < zero_row; ++row) {
            double factor = serie.at({ row, 0 });
            double multiplier = 1.0;
            for (int i = 1; i < row + 1; ++i) {
                // cppcheck-suppress useStlAlgorithm
                multiplier *= static_cast<double>(this->num_column - i + 1) / i;
            }
            spdlog::debug("row={} factor={} multiplier={}\n", row, factor, multiplier);
            value += factor * multiplier;
        }
        spdlog::debug("get_next_value={}", value);
        return value;
    }

    double get_prev_value() {
        double value = 0;
        for (int row = 0; row < zero_row; ++row) {
            double factor = serie.at({ row, 0 });
            double multiplier = 1.0;
            for (int i = 1; i < row + 1; ++i) {
                // cppcheck-suppress useStlAlgorithm
                multiplier *= static_cast<double>(-1 - i + 1) / i;
            }
            spdlog::debug("row={} factor={} multiplier={}\n", row, factor, multiplier);
            value += factor * multiplier;
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
    double sum = 0;
    while (std::getline(input, line)) {

        Serie serie(line);
        sum += serie.get_prev_value();
    }
    fmt::print("Total sum = {}\n", sum);
}

int main() {
    using namespace std::chrono;
    // spdlog::set_level(spdlog::level::debug);// Set global log level to debug
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
