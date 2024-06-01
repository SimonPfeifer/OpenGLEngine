add_library(stb_image "")

target_sources(stb_image
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/stb_image.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/stb_image.h
  )

target_include_directories(stb_image
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )