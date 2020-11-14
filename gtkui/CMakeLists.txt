#This file is part of Clear Zone.
#
#Clear Zone is free software : you can redistribute it and / or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#Clear Zone is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with Clear Zone.If not, see < https:  // www.gnu.org/licenses/>.

PROJECT(ClearZoneGtkUI C CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CZG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(CZG_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
SET(STB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../stb/stb")

#Include the PkgConfig utility to glob in GTK3 +
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

INCLUDE_DIRECTORIES(${CZG_INCLUDE} ${GTK3_INCLUDE_DIRS} ${CZC_INCLUDE} ${STB_INCLUDE})
LINK_DIRECTORIES("${GTK3_LIBRARY_DIRS}")
ADD_DEFINITIONS("${GTK3_CFLAGS_OTHER}")

#Set new source files here, boys - j.n.
SET(CZG_SOURCES 
  ${CZG_SRC}/main.cpp 
  ${CZG_SRC}/world_shim.cpp
  ${CZG_SRC}/animation.c 
  ${CZG_SRC}/panic.c 
  ${CZG_SRC}/stb_impl.c
  ${CZG_SRC}/ui.c
)

ADD_EXECUTABLE(ClearZoneGtk ${CZG_SOURCES})
TARGET_LINK_LIBRARIES(ClearZoneGtk ${GTK3_LIBRARIES} ClearZoneCore)
