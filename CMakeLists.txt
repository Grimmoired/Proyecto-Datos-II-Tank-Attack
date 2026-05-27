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
        logica/mapa/Casilla.cpp
        logica/mapa/Casilla.h
        logica/mapa/Mapa.cpp
        logica/mapa/Mapa.h
        logica/rendering/Renderer.cpp
        logica/rendering/Renderer.h
        logica/ui/PantallaMenu.cpp
        logica/ui/PantallaMenu.h
        logica/ui/PantallaInstrucciones.cpp
        logica/ui/PantallaInstrucciones.h
        logica/estructuras/Matriz.h
        logica/estructuras/ArregloDinamico.h
        logica/juego/EstadoJuego.h
        logica/ui/HUD.cpp
        logica/ui/HUD.h
        logica/juego/Tanque.cpp
        logica/juego/Tanque.h
        logica/juego/GestorTurnos.cpp
        logica/juego/GestorTurnos.h
        logica/juego/bullet.cpp
        logica/juego/bullet.h
        logica/estructuras/Cola.h
        logica/juego/Powerup.h
        logica/juego/SpawnPowerUp.cpp
        logica/juego/SpawnPowerUp.h
        logica/juego/GameManager.cpp
        logica/juego/GameManager.h
        logica/estructuras/pair.h
        logica/estructuras/priority_queue.h
        logica/pathfinding/pathfinder.h
        logica/pathfinding/pathfinder.cpp
        logica/ui/PantallaVictoria.cpp
        logica/ui/PantallaVictoria.h
        logica/rendering/AnimacionExplosion.cpp
        logica/rendering/AnimacionExplosion.h
        logica/rendering/GestorExplosiones.cpp
        logica/rendering/GestorExplosiones.h
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