cmake_minimum_required(VERSION 3.22)
project(Oracle_Padding_Attack)

set(CMAKE_CXX_STANDARD 23)

add_executable(Oracle_Padding_Attack main.cpp main.h network.h network.cpp)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG db351ffbbadc6c4e9239daaa26e9aefa9f0ec82d)
FetchContent_MakeAvailable(cpr)

target_link_libraries(Oracle_Padding_Attack PRIVATE cpr::cpr)