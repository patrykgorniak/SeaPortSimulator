PROJECT(project)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#TODO: Add dependencies: ImageMagick,SDL-image,copy needed files to destination directory

include_directories(/usr/include/ImageMagick/)
find_package(Magick++)
find_package(SDL)

ADD_EXECUTABLE(project ship.cpp demo.cpp map.cpp main.cpp)
target_link_libraries(project Magick++ SDL SDL_image)
