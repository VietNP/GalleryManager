cmake_minimum_required(VERSION 3.10)
project(GalleryApp CXX)

set(CMAKE_CXX_STANDARD 17)

# Tìm thư viện OpenCV và MySQL
find_package(OpenCV REQUIRED)
find_library(MYSQL_LIB mysqlclient)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(GalleryApp
    main.cpp
    OpenCVImageProcessor.cpp
    GalleryManager.cpp
    DatabaseRepository.cpp
)

target_link_libraries(GalleryApp ${OpenCV_LIBS} ${MYSQL_LIB})