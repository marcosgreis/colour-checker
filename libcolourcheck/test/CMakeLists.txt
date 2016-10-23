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

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${libcolourcheck_SOURCE_DIR}/src 
                    ${Boost_INCLUDE_DIRS})

add_definitions(-DBOOST_TEST_DYN_LINK)

add_executable(colourtest
               colourfunction.cpp)
target_link_libraries(colourtest
                      colourcheck
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME ColourFcnTest COMMAND colourtest)

add_executable(avgfiltertest
               avgfilter.cpp)
target_link_libraries(avgfiltertest
                      colourcheck
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(NAME AverageFilterTest COMMAND avgfiltertest)

