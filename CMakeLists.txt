project(book-recognition-with-surf)
cmake_minimum_required(VERSION 2.8)

add_definitions("-std=c++11 -Wall")

find_package(OpenCV REQUIRED)

add_subdirectory(book-recognition-lib)

set(book_SOURCES
  src/main.cpp)

add_executable(book-recognition-with-surf ${book_SOURCES})
target_link_libraries(book-recognition-with-surf ${OpenCV_LIBS} book-recognition-lib)

#install(TARGETS book-recognition-with-surf RUNTIME DESTINATION bin)