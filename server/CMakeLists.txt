# message("Sources = https://github.com/olegskip/dino")
cmake_minimum_required(VERSION 3.7.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wextra -pthread")

project(battleship_server)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(${PROJECT_NAME} 
${PROJECT_SOURCE_DIR}/main.cpp 
${PROJECT_SOURCE_DIR}/TCPServer.h
${PROJECT_SOURCE_DIR}/TCPServer.cpp
${PROJECT_SOURCE_DIR}/TCPConnection.h
${PROJECT_SOURCE_DIR}/TCPConnection.cpp
${PROJECT_SOURCE_DIR}/room.h
${PROJECT_SOURCE_DIR}/room.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)