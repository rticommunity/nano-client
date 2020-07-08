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

project(nano-arduino-c
    LANGUAGES   C CXX
    VERSION     0.1.0
    DESCRIPTION "A C wrapper for (some of) Arduino's C++ API")

################################################################################
include(../../core/resource/cmake/nano-build.cmake)
################################################################################

nano_project()

if (${PROJECT_NAME}_BUILD_LIBRARIES)
message(WARNING "This package will likely fail to build stand-alone, and it should only be used to copy the included source and header files.")
endif()

################################################################################
# "arduinoc" component
################################################################################

set(NANO_ARDUINO_C_INCLUDE_FILES_PUBLIC
        include/nano/nano_arduino_c.h
        include/nano/nano_arduino_c_serial.h)

set(NANO_ARDUINO_C_SOURCE_FILES
        src/NanoArduinoSerial.cpp)

set(NANO_ARDUINO_C_LIBRARY               arduinoc)

nano_component(NANO_ARDUINO_C)
nano_component_library(NANO_ARDUINO_C)
nano_component_install(NANO_ARDUINO_C)

################################################################################

nano_project_install()

################################################################################

nano_project_summary()

################################################################################