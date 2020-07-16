################################################################################
#
# (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

cmake_minimum_required(VERSION 3.9)

################################################################################

project(nano-client-arduino
    LANGUAGES   CXX
    VERSION     0.1.0
    DESCRIPTION "A C++, Arduino-compatible, XRCE Client API")

################################################################################
include(../../core/resource/cmake/nano-build.cmake)
################################################################################

# This project does not have its own manual, only API docs
set(BUILD_DOCS_MANUAL       OFF)

# This project cannot be built with CMake at the moment
set(BUILD_LIBRARIES         OFF)
set(BUILD_EXECUTABLES       OFF)
set(INSTALL_SOURCE          OFF)

nano_project()

################################################################################

function(build_nano_client_cpp)
    add_subdirectory(../nano-client-cpp cpp)
endfunction()

if(NOT TARGET nano::nanocppclient)
    message(STATUS "Building nano-client-cpp for ${PROJECT_NAME}")
    build_nano_client_cpp()
endif()

################################################################################

set(NANO_CLIENT_ARDUINO_INCLUDE_FILES_PUBLIC
        src/nano_client_arduino.h)

nano_component(NANO_CLIENT_ARDUINO)

# Unset these variables so that they may be picked up from the cache
unset(NANO_CLIENT_ARDUINO_INCLUDE_FILES_PUBLIC)

################################################################################

if (${PROJECT_NAME}_BUILD_DOCS)

    set(DOC_INPUT_DIRS  ${PROJECT_SOURCE_DIR}/src)

    set(DOC_INPUT_SRC   ${NANO_CLIENT_ARDUINO_INCLUDE_FILES_PUBLIC})
    
    set(DOC_DEPS        nano-client-cpp)

    nano_project_docs()
endif()

################################################################################

nano_project_install()

################################################################################

nano_project_summary()

################################################################################