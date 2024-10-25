#include <chrono>
#include <fstream>

#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <ranges>
#include <set>
#include <unordered_map>

// NOLINTNEXTLINE(bugprone-exception-escape)
struct Node
{
    std::string current; // cppcheck-suppress unusedStructMember
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
    std::vector<std::string> currentVector;
    while (!input.eof()) {
        std::string ignore;
        std::string current;
        std::string left;
        std::string right;
        input >> current >> ignore >> left >> right;
        // current=AAA ignore== left=(BBB, right=CCC)
        if (current[2] == 'A') { currentVector.push_back(current); }
        navigateNode[current] = { current, left.substr(1, 3), right.substr(0, 3) };
        spdlog::trace("current={} ignore={} navigateNode.left={} navigateNode.right={}",
          current,
          ignore,
          navigateNode[current].left,
          navigateNode[current].right);
    }

    for (auto &current : currentVector) {
        unsigned step = 0;
        std::string currentSlow = current;
        std::string currentFast = current;
        while (true) {
            auto doStep = [&navigateNode, &navigateCommand](std::string current_, unsigned step_) {
                std::string taget;
                Node node = navigateNode.at(current_);
                unsigned stepCommand = step_ % static_cast<unsigned>(navigateCommand.size());
                if (navigateCommand[stepCommand] == 'L') {
                    taget = node.left;
                } else if (navigateCommand[stepCommand] == 'R') {
                    taget = node.right;
                } else {
                    spdlog::warn("Illegal command in navigateCommand[stepCommand]={}", navigateCommand[stepCommand]);
                }
                spdlog::debug("step={} current={} target={}", step_, current_, taget);
                return taget;
            };

            currentSlow = doStep(currentSlow, step);
            currentFast = doStep(currentFast, 2 * step);
            currentFast = doStep(currentFast, 2 * step + 1);
            spdlog::debug("step={} currentSlow={}", step, currentSlow);
            ++step;

            if (currentSlow == currentFast && ((step % static_cast<unsigned>(navigateCommand.size())) == 0)) { break; }
        }
        fmt::print("circle size={} start current={} end_location ={}\n",
          step,
          current,
          step-1 // known by investigation the input
        );
    }
    fmt::print("The slution is the LCM of all circles.\n");
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
