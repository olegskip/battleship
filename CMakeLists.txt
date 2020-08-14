cmake_minimum_required(VERSION 3.5)

project(battleship LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_executable(battleship
    main.cpp
    main_window.cpp
    main_window.h
    cell.h
    cell.cpp
	board.h
	board.cpp
	ship.h
	ship.cpp
	pop_up_message.cpp
	pop_up_message.h
	game_window.h
	game_window.cpp
	MenuWindow/menu_window.h
	MenuWindow/menu_window.cpp
	MenuWindow/animated_shadow.h
	MenuWindow/animated_shadow.cpp
	Network/client.h
	Network/client.cpp
    config.h
)

target_link_libraries(battleship PRIVATE Qt5::Widgets Qt5::Network)
