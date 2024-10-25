#include <fstream>
#include <chrono>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <advent2023/sample_library.hpp>
#include <advent2023/day5_lib.hpp>

#include <limits>

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream map_file{ dir_path + "\\map.txt" };
    std::ifstream seed_file{ dir_path + "\\seed.txt" };

    auto [mcategory, mdestination] = readFile(map_file);
    // you can earase the the string each read
    unsigned min_location = std::numeric_limits<unsigned>::max();
    while (! seed_file.eof()) {
        
        std::string destination_category {"seed"};
        unsigned destination_value {0};
        seed_file >> destination_value;
        spdlog::debug("solver: seed:{}", destination_value);

        while (destination_category != "location"){
            destination_category = mcategory.at(destination_category);
            spdlog::debug("solver: destination_category={}", destination_category);
            destination_value = getDestintion(mdestination.at(destination_category), destination_value);
            spdlog::debug("solver: destination_value={}", destination_value);
        }

        // destination_value = getDestintion(mdestination.at(destination_category), destination_value);
        spdlog::debug("solver: destination_category={} destination_value={}", destination_category, destination_value);

        min_location = std::min(min_location, destination_value);
    }
    fmt::print("min location={}\n", min_location);
}


int main() {
    using namespace std::chrono;
    spdlog::set_level(spdlog::level::debug);// Set global log level to debug
    // spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

    const time_point<steady_clock> start_time = steady_clock::now();
    try {
        solver();
    } catch (const std::exception &e) { spdlog::error("what: {}", e.what()); }
    const time_point<steady_clock> end_time = std::chrono::steady_clock::now();
    fmt::print("Run time: {}us\n", duration_cast<std::chrono::microseconds>(end_time - start_time).count());
    return 0;
}
