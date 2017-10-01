project(screensaver.stars)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
if (NOT WIN32)
  find_package(OpenGL REQUIRED)
  set(DEPLIBS ${OPENGL_LIBRARIES})
endif()

include_directories(${KODI_INCLUDE_DIR}/..) # Hack way with "/..", need bigger Kodi cmake rework to match right include ways

set(STARS_SOURCES src/StarField.cpp
                  src/Stars.cpp)

set(STARS_HEADERS src/StarField.h
                  src/types.h)

build_addon(screensaver.stars STARS DEPLIBS)

include(CPack)
