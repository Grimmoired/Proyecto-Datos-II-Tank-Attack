cmake_minimum_required(VERSION 3.28)
project(TankAttack)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR "C:/Libs/SFML-2.6.2/lib/cmake/SFML")

find_package(SFML 2.6 COMPONENTS graphics window system audio REQUIRED)

file(GLOB_RECURSE SOURCES "logica/*.cpp")

add_executable(TankAttack ${SOURCES}
        logica/main.cpp
        logica/assets/AssetManager.cpp
        logica/assets/AssetManager.h
)

target_include_directories(TankAttack PRIVATE logica/)

target_link_libraries(TankAttack
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

add_custom_command(TARGET TankAttack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/libs/SFML-2.6.2/bin"
        $<TARGET_FILE_DIR:TankAttack>)

add_custom_command(TARGET TankAttack POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:TankAttack>/assets")