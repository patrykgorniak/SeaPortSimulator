set(SEA_PORT_SRCS
        main.cpp
        demo.cpp
        map.cpp
        ship.cpp)


set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#TODO: Add dependencies: ImageMagick,SDL-image,copy needed files to destination directory

include_directories(/usr/include/ImageMagick/)
#find_package(Magick++)
#find_package(SDL)
add_executable(${PROJECT_NAME} ${SEA_PORT_SRCS})
target_link_libraries(${PROJECT_NAME} Magick++ SDL SDL_image)
