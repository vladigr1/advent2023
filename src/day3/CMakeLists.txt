add_executable(day3 main.cpp)

add_library(advent2023::day3 ALIAS sample_library)

target_link_libraries(
    day3
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
    day3
  PRIVATE
          fmt::fmt
          spdlog::spdlog)