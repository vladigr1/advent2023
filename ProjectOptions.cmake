include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(advent2023_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(advent2023_setup_options)
  option(advent2023_ENABLE_HARDENING "Enable hardening" ON)
  option(advent2023_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    advent2023_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    advent2023_ENABLE_HARDENING
    OFF)

  advent2023_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR advent2023_PACKAGING_MAINTAINER_MODE)
    option(advent2023_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(advent2023_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(advent2023_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(advent2023_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(advent2023_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(advent2023_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(advent2023_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(advent2023_ENABLE_PCH "Enable precompiled headers" OFF)
    option(advent2023_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(advent2023_ENABLE_IPO "Enable IPO/LTO" ON)
    option(advent2023_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(advent2023_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(advent2023_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(advent2023_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(advent2023_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(advent2023_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(advent2023_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(advent2023_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(advent2023_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(advent2023_ENABLE_PCH "Enable precompiled headers" OFF)
    option(advent2023_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      advent2023_ENABLE_IPO
      advent2023_WARNINGS_AS_ERRORS
      advent2023_ENABLE_USER_LINKER
      advent2023_ENABLE_SANITIZER_ADDRESS
      advent2023_ENABLE_SANITIZER_LEAK
      advent2023_ENABLE_SANITIZER_UNDEFINED
      advent2023_ENABLE_SANITIZER_THREAD
      advent2023_ENABLE_SANITIZER_MEMORY
      advent2023_ENABLE_UNITY_BUILD
      advent2023_ENABLE_CLANG_TIDY
      advent2023_ENABLE_CPPCHECK
      advent2023_ENABLE_COVERAGE
      advent2023_ENABLE_PCH
      advent2023_ENABLE_CACHE)
  endif()

  advent2023_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (advent2023_ENABLE_SANITIZER_ADDRESS OR advent2023_ENABLE_SANITIZER_THREAD OR advent2023_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(advent2023_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(advent2023_global_options)
  if(advent2023_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    advent2023_enable_ipo()
  endif()

  advent2023_supports_sanitizers()

  if(advent2023_ENABLE_HARDENING AND advent2023_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR advent2023_ENABLE_SANITIZER_UNDEFINED
       OR advent2023_ENABLE_SANITIZER_ADDRESS
       OR advent2023_ENABLE_SANITIZER_THREAD
       OR advent2023_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${advent2023_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${advent2023_ENABLE_SANITIZER_UNDEFINED}")
    advent2023_enable_hardening(advent2023_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(advent2023_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(advent2023_warnings INTERFACE)
  add_library(advent2023_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  advent2023_set_project_warnings(
    advent2023_warnings
    ${advent2023_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(advent2023_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(advent2023_options)
  endif()

  include(cmake/Sanitizers.cmake)
  advent2023_enable_sanitizers(
    advent2023_options
    ${advent2023_ENABLE_SANITIZER_ADDRESS}
    ${advent2023_ENABLE_SANITIZER_LEAK}
    ${advent2023_ENABLE_SANITIZER_UNDEFINED}
    ${advent2023_ENABLE_SANITIZER_THREAD}
    ${advent2023_ENABLE_SANITIZER_MEMORY})

  set_target_properties(advent2023_options PROPERTIES UNITY_BUILD ${advent2023_ENABLE_UNITY_BUILD})

  if(advent2023_ENABLE_PCH)
    target_precompile_headers(
      advent2023_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(advent2023_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    advent2023_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(advent2023_ENABLE_CLANG_TIDY)
    advent2023_enable_clang_tidy(advent2023_options ${advent2023_WARNINGS_AS_ERRORS})
  endif()

  if(advent2023_ENABLE_CPPCHECK)
    advent2023_enable_cppcheck(${advent2023_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(advent2023_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    advent2023_enable_coverage(advent2023_options)
  endif()

  if(advent2023_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(advent2023_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(advent2023_ENABLE_HARDENING AND NOT advent2023_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR advent2023_ENABLE_SANITIZER_UNDEFINED
       OR advent2023_ENABLE_SANITIZER_ADDRESS
       OR advent2023_ENABLE_SANITIZER_THREAD
       OR advent2023_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    advent2023_enable_hardening(advent2023_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
