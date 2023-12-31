add_executable(day4_b main.cpp)

target_link_libraries(
    day4_b
  PRIVATE advent2023_options
          advent2023_warnings
          advent2023::sample_library)

target_link_system_libraries(
    day4_b
  PRIVATE
          fmt::fmt
          spdlog::spdlog)