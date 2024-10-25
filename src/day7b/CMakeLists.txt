get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})

add_executable(${ProjectId} main.cpp)

target_link_libraries(
    ${ProjectId}
  PRIVATE advent2023_options
          advent2023_warnings)

target_link_system_libraries(
    ${ProjectId}
  PRIVATE
          fmt::fmt
          spdlog::spdlog
          magic_enum::magic_enum)