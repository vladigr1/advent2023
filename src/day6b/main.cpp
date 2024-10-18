#include <fstream>
#include <chrono>
#include <limits>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <cmath>

uint64_t solution(uint64_t time, uint64_t distance){
    for (uint64_t speedup=0; speedup<=time; ++speedup){
        if (speedup*(time-speedup) >  distance){
            spdlog::debug("speedup={}\n", speedup);
            return (time-speedup) - speedup + 1;
        }
    }
    return 0;
}

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream time_file{ dir_path + "\\time.txt" };
    std::ifstream distnace_file{ dir_path + "\\distance.txt" };

    uint64_t total = 1;
    while ( !time_file.eof()) {
        uint64_t time, distance, sol;
        time_file >> time;
        distnace_file >> distance;
        spdlog::debug("time={}\n", time);
        spdlog::debug("distance={}\n", distance);
        sol = solution(time, distance);
        spdlog::debug("sol={}\n", sol);
        if (sol != 0){
            total *= sol;
        }
    }
    fmt::print("total solutions: {}\n", total);
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
