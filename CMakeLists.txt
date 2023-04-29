cmake_minimum_required(VERSION 3.24)
project(MenadzerHasel)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY  https://github.com/fmtlib/fmt
        GIT_TAG         a33701196adfad74917046096bf5a2aa0ab0bb50
)
FetchContent_MakeAvailable(fmt)

add_executable(MenadzerHasel main.cpp main2.cpp)
target_link_libraries(MenadzerHasel fmt)
