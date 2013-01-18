project(book-recognition-with-surf)
cmake_minimum_required(VERSION 2.8)


add_definitions("-std=c++11 -Wall -g")

find_package(OpenCV REQUIRED)
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-2.4)

add_subdirectory(book-recognition-lib)

include_directories(src/ include/ book-recognition-lib ${OpenCV_INCLUDE_DIRS}
  ${GTKMM_INCLUDE_DIRS} )

set(book_SOURCES
  src/main.cpp
  src/MainWindow.cpp
  src/BookDataDialog.cpp)

add_executable(book-recognition ${book_SOURCES})
target_link_libraries(book-recognition  ${OpenCV_LIBS} book-recognition-lib
  ${GTKMM_LIBRARIES} )

#install(TARGETS book-recognition-with-surf RUNTIME DESTINATION bin)
