project(book-recognition-lib)

set(CMAKE_MODULE_PATH "../.cmake")
find_package(TinyXML REQUIRED)

set(book_lib_SOURCES
  Recognizer.cpp
  Database.cpp
  Book.cpp
  Exceptions.cpp)
  

include_directories(${OpenCV_INCLUDE_DIRS} ${TinyXML_INCLUDE_DIR})

add_library(book-recognition-lib ${book_lib_SOURCES})
target_link_libraries(book-recognition-lib ${OpenCV_LIBS} ${TinyXML_LIBRARY})
