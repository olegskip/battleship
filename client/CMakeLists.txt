cmake_minimum_required(VERSION 3.5)

project(battleship LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wextra")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCE_DIR}/main.cpp
	${PROJECT_SOURCE_DIR}/main_window.cpp
	${PROJECT_SOURCE_DIR}/main_window.h
	${PROJECT_SOURCE_DIR}/cell.h
	${PROJECT_SOURCE_DIR}/cell.cpp
	${PROJECT_SOURCE_DIR}/board.h
	${PROJECT_SOURCE_DIR}/board.cpp
	${PROJECT_SOURCE_DIR}/ship.h
	${PROJECT_SOURCE_DIR}/ship.cpp
	${PROJECT_SOURCE_DIR}/pop_up_message.cpp
	${PROJECT_SOURCE_DIR}/pop_up_message.h
	${PROJECT_SOURCE_DIR}/game_window.h
	${PROJECT_SOURCE_DIR}/game_window.cpp
	${PROJECT_SOURCE_DIR}/MenuWindow/menu_window.h
	${PROJECT_SOURCE_DIR}/MenuWindow/menu_window.cpp
	${PROJECT_SOURCE_DIR}/MenuWindow/animated_shadow.h
	${PROJECT_SOURCE_DIR}/MenuWindow/animated_shadow.cpp
        ${PROJECT_SOURCE_DIR}/MenuWindow/rooms_table.h
        ${PROJECT_SOURCE_DIR}/MenuWindow/rooms_table.cpp
	${PROJECT_SOURCE_DIR}/Network/client.h
	${PROJECT_SOURCE_DIR}/Network/client.cpp
	${PROJECT_SOURCE_DIR}/config.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Network)
