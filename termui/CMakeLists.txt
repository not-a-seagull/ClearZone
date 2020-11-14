# This file is part of Clear Zone.
#
# Clear Zone is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Clear Zone is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Clear Zone.  If not, see <https://www.gnu.org/licenses/>.

PROJECT(ClearZoneTermUI CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CZT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(CZT_SRC     "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Include the PkgConfig utility to glob in GTK3+
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(CURSES REQUIRED ncurses)

INCLUDE_DIRECTORIES("${CZT_INCLUDE} ${CURSES_INCLUDE_DIRS} ${CZC_INCLUDE}")
LINK_DIRECTORIES("${CURSES_LIBRARY_DIRS}")
ADD_DEFINITIONS("${CURSES_CFLAGS_OTHER}")

# Set new source files here, boys - j.n.
SET(CZT_SOURCES
  ${CZT_SRC}/main.cpp
)

ADD_EXECUTABLE(ClearZone ${CZT_SOURCES})
TARGET_LINK_LIBRARIES(ClearZone ${CURSES_LIBRARIES} ClearZoneCore)
