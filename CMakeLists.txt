# CMake build script for Clear Zone
# 
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

PROJECT(ClearZone CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CMAKE_C_STANDARD 11)
SET(CMAKE_CXX_STANDARD 17)

# By default, we pretty much need to include the core
ADD_SUBDIRECTORY(core)

# Figure out which interface to bind to.
SET(UI_NAME "gtkui" CACHE STRING "UI to build ClearZone with")
SET_PROPERTY(CACHE UI_NAME PROPERTY STRINGS "gtkui" "termui" "webui")
MESSAGE("-- UI_NAME is ${UI_NAME}")

IF (UI_NAME STREQUAL "gtkui")
  ADD_SUBDIRECTORY(gtkui)
ELSEIF (UI_NAME STREQUAL "termui")
  ADD_SUBDIRECTORY(termui)
ELSEIF (UI_NAME STREQUAL "webui")
  ADD_SUBDIRECTORY(webui)
ELSE ()
  MESSAGE(FATAL_ERROR "Did not select any known UI frontends")
ENDIF()
