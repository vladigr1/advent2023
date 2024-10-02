#include <fstream>
#include <chrono>
#include <limits>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <advent2023/sample_library.hpp>
#include <advent2023/day5_lib.hpp>

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream map_file{ dir_path + "\\map.txt" };
    std::ifstream seed_file{ dir_path + "\\seed.txt" };
    
    MapFirstRange vseed = vectorSeed(seed_file);
    vseed = mergeVectorSeed(vseed);

    auto [mcategory, mdestination] = readFile(map_file);
    std::string destination_category = "seed";

    while (destination_category != "location") {
        destination_category = mcategory.at(destination_category);
        spdlog::debug("destination_category={}\n",destination_category);
        
        MapFirstRange curMapdist = mdestination.at(destination_category);

        MapFirstRange vdestination = genVectorSeedFromMapDestination(vseed, curMapdist);

        vseed = mergeVectorSeed(vdestination);
        
    }
    fmt::print("min location={}\n", vseed.begin()->first);
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
