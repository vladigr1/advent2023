if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)
    add_executable(day3_benchamrk benchmark.cpp original.cpp)

    target_link_libraries(
        day3_benchamrk
    PRIVATE advent2023_options
            advent2023_warnings)

    target_link_system_libraries(
        day3_benchamrk
    PRIVATE
            fmt::fmt
            spdlog::spdlog
            benchmark::benchmark)


    add_executable(day3_benchamrk_v2 benchmark.cpp link_list_returns.cpp)

    target_link_libraries(
        day3_benchamrk_v2
    PRIVATE advent2023_options
            advent2023_warnings)

    target_link_system_libraries(
        day3_benchamrk_v2
    PRIVATE
            fmt::fmt
            spdlog::spdlog
            benchmark::benchmark)
endif()