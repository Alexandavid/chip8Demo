cmake_minimum_required(VERSION 3.22)
project(DemoChip8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)

include_directories(
        C:/msys64/mingw64/include
        ${SDL2_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
        app/main.cpp
        app/SDLAbstractLayer/HL_SDL.cpp
        app/constants.h
        app/CHIP8.cpp
        app/Peripherials/KeyboardHandler.cpp
        app/Peripherials/GraphicsManager.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        winmm
)

############################ Testing

set(CPPUTEST_LIB_HOME C:/msys64/mingw64/lib/)

if (NOT EXISTS ${CPPUTEST_LIB_HOME}/libCppUTest.a OR NOT EXISTS ${CPPUTEST_LIB_HOME}/libCppUTestExt.a)
    message(FATAL_ERROR "CppUTest libraries not found in ${CPPUTEST_LIB_HOME}")
endif()

# Add Tests
set(TEST_SOURCES
        Tests/main.cpp
        Tests/IntegrationTest.cpp
)

# Define to disable CppUTest's custom operator new

# Define test executable
add_definitions(-DCPPUTEST_MEMORY_LEAK_CHECKER_DISABLED)

add_executable(UnitTests ${TEST_SOURCES})

target_link_libraries(UnitTests
        ${CPPUTEST_LIB_HOME}/libCppUTest.a
        ${CPPUTEST_LIB_HOME}/libCppUTestExt.a
)


enable_testing()
add_test(NAME RunTests COMMAND ${PROJECT_NAME}_tests)