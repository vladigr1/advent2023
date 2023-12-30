add_executable(day4 main.cpp)

target_link_libraries(
    day4
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
    day4
  PRIVATE
          fmt::fmt
          spdlog::spdlog)