project(screensaver.stars)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(xbmc REQUIRED)

include(ExternalProject)

include_directories(${XBMC_INCLUDE_DIR})

set(STARS_SOURCES src/StarField.cpp
                   src/Stars.cpp)

set(DEPLIBS)

build_addon(screensaver.stars STARS DEPLIBS)

include(CPack)
