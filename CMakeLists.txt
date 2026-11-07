cmake_minimum_required(VERSION 3.14)
project(delta_core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(delta_core STATIC
    src/orderbook.cpp
    src/signer.cpp
    src/client.cpp
)

add_executable(delta_demo src/main.cpp)
target_link_libraries(delta_demo PRIVATE delta_core)

enable_testing()
add_executable(run_tests tests/test_delta.cpp)
target_link_libraries(run_tests PRIVATE delta_core)
add_test(NAME DeltaUnitTests COMMAND run_tests)
