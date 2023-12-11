add_executable(day1 main.cpp)

target_link_libraries(
  day1
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
  day1
  PRIVATE
          fmt::fmt
          spdlog::spdlog)