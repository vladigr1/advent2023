include(GenerateExportHeader)


add_library(day5_lib day5_lib.cpp)



add_library(advent2023::day5_lib ALIAS day5_lib)

target_link_libraries(day5_lib PRIVATE advent2023_options advent2023_warnings 
    fmt::fmt
    spdlog::spdlog)

target_include_directories(day5_lib ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(day5_lib PUBLIC cxx_std_20)

set_target_properties(
  day5_lib
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(day5_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/advent2023/day5_lib_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(day5_lib PUBLIC day5_lib_STATIC_DEFINE)
endif()
