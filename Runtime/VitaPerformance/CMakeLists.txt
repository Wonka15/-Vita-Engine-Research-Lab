cmake_minimum_required(VERSION 3.16)
project(VitaPerformanceRuntime VERSION 0.1.0
    DESCRIPTION "Portable C99 performance profiler for PlayStation Vita games"
    LANGUAGES C)

add_library(vpr STATIC src/vpr_profiler.c)
target_include_directories(vpr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(vpr PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)

if(MSVC)
    target_compile_options(vpr PRIVATE /W4)
else()
    target_compile_options(vpr PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(VPR_BUILD_TESTS "Build deterministic VPR host tests" ON)
if(VPR_BUILD_TESTS)
    enable_testing()
    add_executable(vpr_profiler_tests tests/test_profiler.c)
    target_link_libraries(vpr_profiler_tests PRIVATE vpr)
    if(NOT MSVC)
        target_compile_options(vpr_profiler_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME vpr_profiler_tests COMMAND vpr_profiler_tests)
endif()
