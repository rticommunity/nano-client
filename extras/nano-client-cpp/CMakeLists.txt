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

project(nano-client-cpp
    LANGUAGES   CXX
    VERSION     0.1.0
    DESCRIPTION "A C++ XRCE Client API")

################################################################################
include(../../core/resource/cmake/nano-build.cmake)
################################################################################

nano_project()

################################################################################

nano_project_option(
    ENABLE_TRANSPORT_ALL
    "All Transport Plugins"
    ON
    "build all of the included transport plugins")
nano_project_option(
    ENABLE_TRANSPORT_UDPV4
    "UDPv4 Transport Plugins"
    ${${PROJECT_NAME}_ENABLE_TRANSPORT_ALL}
    "build the included UDPv4 transport plugin")
nano_project_option(
    ENABLE_TRANSPORT_SERIAL
    "Serial Transport Plugins"
    ${${PROJECT_NAME}_ENABLE_TRANSPORT_ALL}
    "build the included Serial transport plugin")

################################################################################
# If this library is being included in a build that doesn't already include the
# C Client API, add it to the build with default build options.
################################################################################
function(build_nano_client_c)
    set(ENABLE_TRANSPORT_ALL        ${${PROJECT_NAME}_ENABLE_TRANSPORT_ALL})
    set(ENABLE_TRANSPORT_UDPV4      ${${PROJECT_NAME}_ENABLE_TRANSPORT_UDPV4})
    set(ENABLE_TRANSPORT_SERIAL     ${${PROJECT_NAME}_ENABLE_TRANSPORT_SERIAL})

    add_subdirectory(../.. c)
endfunction()

if(NOT TARGET nano::nanoclient)
    build_nano_client_c()
endif()

################################################################################

set(NANO_CLIENT_CPP_INCLUDE_FILES_PUBLIC
        include/nano/nano_client.hpp
        include/nano/nano_client_c.hpp
        include/nano/nano_client_client.hpp
        include/nano/nano_client_object.hpp
        include/nano/nano_client_stream.hpp
        include/nano/nano_client_transport.hpp
        include/nano/nano_client_xcdr.hpp)

set(NANO_CLIENT_CPP_SOURCE_FILES
        src/xrce/XrceClient.cpp
        src/xrce/XrceObject.cpp
        src/xrce/XrceStream.cpp
        src/xcdr/XcdrStream.cpp)

set(NANO_CLIENT_CPP_LIBS        nano::nanoclient)

set(NANO_CLIENT_CPP_LIBRARY     cppclient)

################################################################################

nano_component(NANO_CLIENT_CPP)
nano_component_library(NANO_CLIENT_CPP)
nano_component_install(NANO_CLIENT_CPP)

################################################################################

set(NANO_CLIENT_CPP_UDPV4_INCLUDE_FILES_PUBLIC
        include/nano/nano_client_transport_udp.hpp)

set(NANO_CLIENT_CPP_UDPV4_SOURCE_FILES
        src/xrce/XrceTransportUdp.cpp)

set(NANO_CLIENT_CPP_UDPV4_LIBS      nano::nanocppclient
                                    nano::nanotransportudpv4)

set(NANO_CLIENT_CPP_UDPV4_LIBRARY   cpptransportudpv4)

################################################################################

if(${PROJECT_NAME}_ENABLE_TRANSPORT_UDPV4)
    nano_component(NANO_CLIENT_CPP_UDPV4)
    nano_component_library(NANO_CLIENT_CPP_UDPV4)
    nano_component_install(NANO_CLIENT_CPP_UDPV4)
endif()

################################################################################

set(NANO_CLIENT_CPP_SERIAL_INCLUDE_FILES_PUBLIC
        include/nano/nano_client_transport_serial.hpp)

set(NANO_CLIENT_CPP_SERIAL_SOURCE_FILES
        src/xrce/XrceTransportSerial.cpp)

set(NANO_CLIENT_CPP_SERIAL_LIBS     nano::nanocppclient
                                    nano::nanotransportserial)

set(NANO_CLIENT_CPP_SERIAL_LIBRARY  cpptransportserial)

################################################################################

if(${PROJECT_NAME}_ENABLE_TRANSPORT_SERIAL)
    nano_component(NANO_CLIENT_CPP_SERIAL)
    nano_component_library(NANO_CLIENT_CPP_SERIAL)
    nano_component_install(NANO_CLIENT_CPP_SERIAL)
endif()

################################################################################

nano_project_install()

################################################################################

nano_project_summary()

################################################################################