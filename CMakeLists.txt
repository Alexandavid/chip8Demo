cmake_minimum_required(VERSION 3.22)
project(DemoChip8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6Core REQUIRED)
find_package(Qt6Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)

include_directories(
        C:/msys64/mingw64/include
        ${SDL2_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} WIN32
        app/main.cpp
        app/Drivers/GraphicsDrivers.cpp
        app/constants.h
        app/CHIP8.cpp
        app/Chip8EmulatorQtWrapper.cpp
        app/Peripherials/KeyboardHandler.cpp
        app/Peripherials/GraphicsManager.cpp
        app/Peripherials/AudioManager.cpp
        app/Qt/MainWindow.cpp
        app/Qt/mainwindow.ui
)

target_link_libraries(${PROJECT_NAME}
        winmm
        Qt6::Core
        Qt6::Widgets
        Qt6::Multimedia
)

# Packaging for GitHub release: To build the release, set the build type to 'Release'
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode")
else()
    message(STATUS "Building in Debug mode")
endif()


# Create the deployment package (these are necessary for the process)
include(InstallRequiredSystemLibraries)
include(CPack)
