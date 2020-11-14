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

PROJECT(ClearZoneWebUI CXX)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(CZW_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(CZW_SRC     "${CMAKE_CURRENT_SOURCE_DIR}/src")

INCLUDE_DIRECTORIES("${CZW_INCLUDE} ${CZC_INCLUDE}")

# Set new source files here, boys - j.n.
SET(CZW_SOURCES
  ${CZW_SRC}/entry.cpp
)

ADD_LIBRARY(ClearZoneWeb SHARED ${CZW_SOURCES})
TARGET_LINK_LIBRARIES(ClearZoneWeb ClearZoneCore)
