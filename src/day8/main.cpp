#include <chrono>
#include <fstream>

#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <ranges>
#include <unordered_map>

struct Node
{
    std::string current;
    std::string left;
    std::string right;
};

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input{ dir_path + "\\input1.txt" };

    std::string navigateCommand;
    std::getline(input, navigateCommand);
    spdlog::debug("navigateCommand={}", navigateCommand);
    std::unordered_map<std::string, Node> navigateNode;
    while (!input.eof()) {
        std::string ignore, current, left, right;
        input >> current >> ignore >> left >> right;
        // current=AAA ignore== left=(BBB, right=CCC)

        spdlog::debug("current={} ignore={} left={} right={}", current, ignore, left, right);
        navigateNode[current] = {current, left.substr(1,3), right.substr(0,3)};
        spdlog::debug("current={} ignore={} navigateNode.left={} navigateNode.right={}", current, ignore, navigateNode[current].left, navigateNode[current].right);
    }

    unsigned step = 0;
    std::string current = "AAA";
    while(current != "ZZZ"){
        spdlog::debug("current={}", current);
        Node node = navigateNode.at(current);
        unsigned stepCommand = step % navigateCommand.size();
        if(navigateCommand[stepCommand] == 'L'){
            current = node.left;
        }else if(navigateCommand[stepCommand] == 'R'){
            current = node.right;
        }else{
            spdlog::warn("Illegal command in navigateCommand[stepCommand]={}", navigateCommand[stepCommand]);
        }
        ++step;
    }
    fmt::print("total solutions: {}\n", step);
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
