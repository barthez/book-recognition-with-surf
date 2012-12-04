find_path(TinyXML_INCLUDE_DIR
  NAMES tinyxml.h
  PATHS /usr/include)
  
find_library(TinyXML_LIBRARY tinyxml
  PATHS /lib 
        /usr/lib 
        /usr/lib32 
        /usr/lib64
        ENV TINYXML_PATH)  
        
if(TinyXML_INCLUDE_DIR AND TinyXML_LIBRARY)
  set(FOUND_TinyXML 1 CACHE BOOL "Found TinyXML")
else(TinyXML_INCLUDE_DIR AND TinyXML_LIBRARY)
  message(ERROR "TinyXML not found")
  set(FOUND_TinyXML 0 CACHE BOOL "TinyXML not found")
endif(TinyXML_INCLUDE_DIR AND TinyXML_LIBRARY)

mark_as_advanced(TinyXML_INCLUDE_DIR TinyXML_LIBRARY FOUND_TinyXML)