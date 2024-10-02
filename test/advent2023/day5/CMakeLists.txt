add_executable(tests5 tests.cpp)
target_link_libraries(
  tests5
  PRIVATE advent2023::advent2023_warnings
          advent2023::advent2023_options
          advent2023::day5_lib
          Catch2::Catch2WithMain)
target_compile_features(tests5 PUBLIC cxx_std_20)

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(
    TARGET tests5
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:tests5> $<TARGET_FILE_DIR:tests5>
    COMMAND_EXPAND_LISTS)
endif()

# automatically discover tests that are defined in catch based test files you can modify the unittests. Set TEST_PREFIX
# to whatever you want, or use different for different binaries
catch_discover_tests(
  tests5
  TEST_PREFIX
  "unittests."
  REPORTER
  XML
  OUTPUT_DIR
  .
  OUTPUT_PREFIX
  "unittests."
  OUTPUT_SUFFIX
  .xml)
