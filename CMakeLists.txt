cmake_minimum_required(VERSION 2.8)
project(SeaPortSimulator)
#add_subdirectory(src)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set(SEA_PORT_SRCS
        src/main.cpp
        src/demo.cpp
        src/map.cpp
        src/ship.cpp)


set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -DMAGICKCORE_HDRI_ENABLE=0 -DMAGICKCORE_QUANTUM_DEPTH=8")


find_package(ImageMagick COMPONENTS Magick++)
find_package(SDL)
find_package(SDL_image)

include_directories(${ImageMagick_INCLUDE_DIRS} ${SDL_INCLUDE_DIRS} ${SDL_IMAGE_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SEA_PORT_SRCS})
target_link_libraries(${PROJECT_NAME} ${ImageMagick_LIBRARIES} ${SDL_IMAGE_LIBRARIES} ${SDL_LIBRARY})

add_subdirectory(resources)
