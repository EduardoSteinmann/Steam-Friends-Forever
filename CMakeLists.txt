cmake_minimum_required(VERSION 3.22)
project(Project3)

set(CMAKE_CXX_STANDARD 17)

#compile flags to match Gradescope test environment
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror") # remove -Wall if you don't want as many warnings treated as errors
# gradescope does use -Werror, so if you remove it here you may run into issues when trying to submit
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(include, lib, src)

add_executable(main
        src/main.cpp
        src/Steam.cpp
        src/AdjacencyList.cpp
        src/AdjacencyMatrix.cpp
        src/Utils.cpp
        src/AppDriver.c
        # src/Game.cpp
        # include/AdjacencyList.hpp   
        # include/AdjacencyMatrix.hpp   
        # include/Game.hpp    
        # include/Steam.hpp   
        # include/SteamUser.hpp
)
target_link_directories(main PUBLIC lib)
target_include_directories(main PUBLIC include lib src)
target_link_libraries(main libcurl SDL3 SDL3_ttf) #link catch to test.cpp file