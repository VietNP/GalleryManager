cmake_minimum_required(VERSION 3.10)
project(GallerySolution CXX)

# Thêm các thư mục module vào chuỗi biên dịch
add_subdirectory(core)
add_subdirectory(database)
add_subdirectory(app)