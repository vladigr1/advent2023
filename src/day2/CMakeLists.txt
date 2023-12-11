add_executable(day2 main.cpp)

target_link_libraries(
  day2
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
  day2
  PRIVATE
          fmt::fmt
          spdlog::spdlog)