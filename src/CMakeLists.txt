cmake_minimum_required(VERSION 2.8)

set(SEA_PORT_SRCS
        main.cpp
        demo.cpp
        map.cpp
        ship.cpp)


set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(SDL)
find_package(SDL_image)

include_directories(${SDL_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SEA_PORT_SRCS})
target_link_libraries(${PROJECT_NAME} ${SDL_IMAGE_LIBRARIES} ${SDL_LIBRARY})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
