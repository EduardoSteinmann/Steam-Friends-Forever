cmake_minimum_required(VERSION 3.22)
project(Steam-Friends-Forever)

set(CMAKE_CXX_STANDARD 17)

#compile flags to match Gradescope test environment
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror") # remove -Wall if you don't want as many warnings treated as errors
# gradescope does use -Werror, so if you remove it here you may run into issues when trying to submit
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

include_directories(include, lib, src)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Steam.cpp
        src/AdjacencyList.cpp
        src/AdjacencyMatrix.cpp
        src/Common.c
        src/AppDriver.c
        # src/Game.cpp
        # include/AdjacencyList.hpp   
        # include/AdjacencyMatrix.hpp   
        # include/Game.hpp    
        # include/Steam.hpp   
        # include/SteamUser.hpp
)
target_link_directories(${PROJECT_NAME} PUBLIC lib)
target_include_directories(${PROJECT_NAME} PUBLIC include lib src)
target_link_libraries(${PROJECT_NAME} PRIVATE libcurl raylib) #link catch to test.cpp file

# Optional: platform-specific libraries (usually handled by raylib)
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32)
elseif(APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(COREVIDEO CoreVideo)
    find_library(OpenGL OpenGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA} ${IOKIT} ${COREVIDEO} ${OpenGL})
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL X11 pthread dl m)
endif()