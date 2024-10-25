#include <chrono>
#include <fstream>

#include <fmt/format.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

// cppcheck-suppress-begin [functionStatic, noExplicitConstructor, shadowFunction, functionStatic, noExplicitConstructor]
#include <magic_enum.hpp>
// cppcheck-suppress-end [functionStatic, noExplicitConstructor, shadowFunction, functionStatic, noExplicitConstructor]

#include <string>

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <set>

enum class Type { HighCard, OnePair, TwoPair, ThreeOfKind, FullHouse, FourOfKind, FiveOfKind };

// NOLINTNEXTLINE(bugprone-exception-escape)
class Player
{
  public:

    unsigned bid;
    std::string hand;
    Player(std::string &hand_, unsigned bid_) : bid(bid_), hand(hand_) {
        std::map<unsigned, unsigned> countCard;

        handValue = 0;
        for (char c : hand_) {
            unsigned cv = cardValue(c);
            handValue *= NUM_CARD_VALUE;
            handValue += cv;


            auto iter = countCard.find(cv);
            if (iter != countCard.end()) {
                ++(iter->second);
            } else {
                countCard[cv] = 1;
            }
        }

        // count for type
        std::array<unsigned, 2> maxCount{ 0 };
        for (std::pair count : countCard) {
            // joker
            if (count.first == 1) { continue; }
            for (unsigned &max_count : maxCount) {
                if (count.second > max_count) {
                    unsigned temp_count = max_count;
                    max_count = count.second;
                    count.second = temp_count;
                }
            }
        }
        auto iter = countCard.find(1);
        if (iter != countCard.end()) { maxCount[0] += iter->second; }

        auto toType = [&maxCount]() {
            switch (maxCount[0]) {
            case 5:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                return Type::FiveOfKind;
            case 4:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                return Type::FourOfKind;
            case 3:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                switch (maxCount[1]) {
                case 2:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                    return Type::FullHouse;
                default:
                    return Type::ThreeOfKind;
                }
            case 2:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                switch (maxCount[1]) {
                case 2:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                    return Type::TwoPair;
                default:
                    return Type::OnePair;
                }
            case 1:// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
                return Type::HighCard;
            default:
                spdlog::debug("fail to find type");
                return Type::OnePair;
            }
        };

        type = toType();
    }

    bool operator<(const Player &other) const {
        if (type < other.type) { return true; }

        if (type == other.type) { return handValue < other.handValue; }

        return false;
    }

    Type type;
    // value for highcard
    unsigned handValue;

    static constexpr unsigned NUM_CARD_VALUE{ 16 };
    static unsigned cardValue(char card) {
        switch (card) {
        case 'T':
            return unsigned{ 10 };// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        case 'J':
            return unsigned{ 1 };// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        case 'Q':
            return unsigned{ 12 };// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        case 'K':
            return unsigned{ 13 };// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        case 'A':
            return unsigned{ 14 };// NOLINT(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        default:
            return static_cast<unsigned>(card - '0');
        }
    }
};

void solver() {
    const std::string file_path = __FILE__;
    const std::string dir_path = file_path.substr(0, file_path.rfind('\\'));
    std::ifstream input{ dir_path + "\\input1.txt" };

    std::vector<Player> player;
    while (!input.eof()) {
        std::string hand;
        unsigned bid{ 0 };
        input >> hand >> bid;
        player.emplace_back(hand, bid);
    }
    std::sort(player.begin(), player.end());

    uint64_t total = 0;
    unsigned rank = 1;
    for (auto iter = player.begin(); iter != player.end(); ++iter) {
        total += rank * iter->bid;
        spdlog::debug("hand={} bid={} rank={} type={} handvalue={:x}\n",
          iter->hand,
          iter->bid,
          rank,
          magic_enum::enum_name(iter->type),
          iter->handValue);
        ++rank;
    }
    fmt::print("total solutions: {}\n", total);
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
