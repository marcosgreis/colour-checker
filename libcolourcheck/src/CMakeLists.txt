# Copyright (C) 2016  Marcos Goulart Reis. All rights reserved.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )

add_library(colourcheck
            fileline.cpp
            spectrometer.cpp
            colourfunction.cpp
           )

add_executable(cmdline-checker main.cpp)
target_link_libraries(cmdline-checker
                      colourcheck ${Boost_LIBRARIES})

add_custom_command(TARGET cmdline-checker POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy 
                   "${CMAKE_CURRENT_BINARY_DIR}/cmdline-checker"
                   "${CMAKE_BINARY_DIR}/cmdline-checker")
