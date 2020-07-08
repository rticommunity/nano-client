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

project(nano-client
    LANGUAGES   C
    VERSION     0.1.0
    DESCRIPTION "An XRCE Client API")

################################################################################

if(NOT TARGET nano::nanocore)
    message(STATUS "ENABLE_XRCE_AGENT := ${ENABLE_XRCE_AGENT}")
    message(STATUS "nano-core_ENABLE_XRCE_AGENT := ${nano-core_ENABLE_XRCE_AGENT}")
    message(STATUS "nano-core-agent_ENABLE_XRCE_AGENT := ${nano-core-agent_ENABLE_XRCE_AGENT}")
    add_subdirectory(core)
endif()

################################################################################
include(core/resource/cmake/nano-build.cmake)
################################################################################

set(EXAMPLES        sensor
                    shape)

set(PREFIX_PATH     ${PROJECT_BINARY_DIR}/core)

set(AUTHOR          "Real-Time Innovations")
set(COPYRIGHT       "2020, Real-Time Innovations, Inc.")
set(GIT_REPO        "https://github.com/rticommunity/nano-client.git")
set(LICENSE         "Apache-2.0")
################################################################################

nano_project()

################################################################################

nano_project_option(
    ENABLE_EXAMPLES
    "Enable Examples"
    OFF
    "build the included examples")
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
# "client" component
################################################################################

set(NANO_CLIENT_INCLUDE_FILES_PUBLIC
        include/nano/nano_client.h
        include/nano/nano_client_config.h)

set(NANO_CLIENT_SOURCE_FILES
        src/Client.c)

set(NANO_CLIENT_LIBS            nano::nanocore)

set(NANO_CLIENT_LIBRARY         client)

nano_component(NANO_CLIENT)

unset(NANO_CLIENT_INCLUDE_FILES_PUBLIC)

nano_component_library(NANO_CLIENT)
nano_component_install(NANO_CLIENT)

################################################################################
# "clientudpv4" component
################################################################################

set(NANO_CLIENT_UDPV4_INCLUDE_FILES_PUBLIC
        include/nano/nano_client_udpv4.h)

set(NANO_CLIENT_UDPV4_SOURCE_FILES
        src/ClientUdp.c)

set(NANO_CLIENT_UDPV4_LIBS      nano::nanoclient
                                nano::nanotransportudpv4)

set(NANO_CLIENT_UDPV4_LIBRARY   clientudpv4)

if (${PROJECT_NAME}_ENABLE_TRANSPORT_UDPV4)
    nano_component(NANO_CLIENT_UDPV4)
    nano_component_library(NANO_CLIENT_UDPV4)
    nano_component_install(NANO_CLIENT_UDPV4)
    unset(NANO_CLIENT_UDPV4_INCLUDE_FILES_PUBLIC)
endif()

################################################################################
# "clientserial" component
################################################################################

set(NANO_CLIENT_SERIAL_INCLUDE_FILES_PUBLIC
        include/nano/nano_client_serial.h)

set(NANO_CLIENT_SERIAL_SOURCE_FILES
        src/ClientSerial.c)

set(NANO_CLIENT_SERIAL_LIBS     nano::nanoclient
                                nano::nanotransportserial)

set(NANO_CLIENT_SERIAL_LIBRARY  clientserial)

if (${PROJECT_NAME}_ENABLE_TRANSPORT_SERIAL)
    nano_component(NANO_CLIENT_SERIAL)
    nano_component_library(NANO_CLIENT_SERIAL)
    nano_component_install(NANO_CLIENT_SERIAL)
    unset(NANO_CLIENT_SERIAL_INCLUDE_FILES_PUBLIC)
endif()

################################################################################

if (${PROJECT_NAME}_BUILD_DOCS)


    set(DOC_INPUT_DIRS  ${PROJECT_SOURCE_DIR}/include/
                        ${PROJECT_SOURCE_DIR}/core/include)

    set(DOC_INPUT_SRC   ${NANO_CLIENT_INCLUDE_FILES_PUBLIC}
                        ${NANO_CORE_INCLUDE_FILES_PUBLIC}
                        ${NANO_TRANSPORT_UDPV4_INCLUDE_FILES_PUBLIC}
                        ${NANO_TRANSPORT_SERIAL_INCLUDE_FILES_PUBLIC})

    set(DOC_SOURCES     index.rst
                        installation.rst
                        building.rst
                        xrce.rst
                        api_manual.rst
                        platform.rst
                        transport.rst
                        vars.rst
                        api_ref.rst)
    
    nano_project_docs()
endif()

################################################################################

nano_project_install()

################################################################################

if(${PROJECT_NAME}_ENABLE_EXAMPLES)
    nano_project_examples()
endif()

################################################################################

nano_project_summary()
