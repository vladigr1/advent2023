add_executable(day3 main.cpp)

target_link_libraries(
    day3
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
    day3
  PRIVATE
          fmt::fmt
          spdlog::spdlog)