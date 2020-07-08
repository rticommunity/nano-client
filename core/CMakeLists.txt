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

if(NOT ENABLE_XRCE_AGENT)
project(nano-core
    LANGUAGES   C
    VERSION     0.1.0
    DESCRIPTION "A portable implementation of the XRCE wire protocol")
else()
project(nano-core-agent
    LANGUAGES   C
    VERSION     0.1.0
    DESCRIPTION "A portable implementation of the XRCE wire protocol (Agent version)")
endif()

################################################################################
include(resource/cmake/nano-build.cmake)
################################################################################

nano_project()

################################################################################

nano_project_option(
    ENABLE_XRCE_AGENT
    "Agent Features"
    OFF
    "build version of libraries suitable for an XRCE Agent")
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
nano_project_option(
    ENABLE_ARDUINO
    "Arduino Support"
    OFF
    "include additional Arduino support")

################################################################################

if(${PROJECT_NAME}_ENABLE_XRCE_AGENT)
    set(agent_suffix             "agent")
    set(lib_prefix              NANO_CORE_AGENT)

    if (${PROJECT_NAME}_BUILD_LIBRARIES)
        nano_project_load_connextdds()
    endif()
else()
    set(agent_suffix             "")
    set(lib_prefix              NANO_CORE)
    set(W_ALL                    true)
endif()

################################################################################

if (${PROJECT_NAME}_ENABLE_ARDUINO)
    # since the arduino-c package should not be used to build a library,
    # we don't check for whether nano::nanoarduinoc is defined as target,
    # and instead we always include it as a subdirectory
    add_subdirectory(../extras/arduino-c arduino-c)
endif()

################################################################################
# "core" component
################################################################################

set(${lib_prefix}_INCLUDE_FILES_PUBLIC
        include/nano/nano_core.h
        include/nano/nano_core_config.h
        include/nano/nano_core_user_config.h
        include/nano/nano_core_osapi_cc_stdc.h
        include/nano/nano_core_osapi_cc.h
        include/nano/nano_core_osapi_net_lwip.h
        include/nano/nano_core_osapi_net_socket.h
        include/nano/nano_core_osapi_net_win.h
        include/nano/nano_core_osapi_platform_arduino.h
        include/nano/nano_core_osapi_platform_posix.h
        include/nano/nano_core_osapi_platform_win.h
        include/nano/nano_core_osapi_seq_decl.h
        include/nano/nano_core_osapi_seq_defn.h
        include/nano/nano_core_osapi_seq_end.h
        include/nano/nano_core_osapi_seq_prelude.h
        include/nano/nano_core_osapi_serial_arduino.h
        include/nano/nano_core_osapi_serial_std.h
        include/nano/nano_core_osapi_serial_win.h
        include/nano/nano_core_osapi.h
        include/nano/nano_core_cdr_basic.h
        include/nano/nano_core_cdr_xrce.h
        include/nano/nano_core_cdr.h
        include/nano/nano_core_log.h
        include/nano/nano_core_log_impl_printf.h
        include/nano/nano_core_log_termcolor.h
        include/nano/nano_core_xrce_basic.h
        include/nano/nano_core_xrce_repr.h
        include/nano/nano_core_xrce_message.h
        include/nano/nano_core_xrce_activity.h
        include/nano/nano_core_xrce_session.h
        include/nano/nano_core_xrce_transport.h
        include/nano/nano_core_xrce.h)

set(${lib_prefix}_SOURCE_FILES
        src/osapi/BufferPool.c
        src/osapi/ClockPosix.c
        src/osapi/ClockWin.c
        src/osapi/Heap.c
        src/osapi/NetSocket.c
        src/osapi/NetSocketWin.c
        src/osapi/NetLwip.c
        src/osapi/Sequence.c
        src/osapi/SerialStd.c
        src/osapi/SerialWin.c
        src/osapi/SerialArduino.c
        src/osapi/Time.c
        src/cdr/CdrActivity.c
        src/cdr/CdrBasic.c
        src/cdr/CdrData.c
        src/cdr/CdrPayload.c
        src/cdr/CdrRepresentation.c
        src/cdr/CdrStream.c
        src/cdr/CdrTime.c
        src/cdr/CdrTransport.c
        src/log/PrintfLog.c
        src/xrce/Basic.c
        src/xrce/Session.c
        src/xrce/Transport.c)

set(${lib_prefix}_INCLUDE_DIRS_PRIVATE
        src/log
        src/xrce)

set(${lib_prefix}_LIBRARY               core${agent_suffix})

if(${PROJECT_NAME}_ENABLE_XRCE_AGENT)
    set(${lib_prefix}_DEFINES           -DNANO_FEAT_AGENT=1)
    list(APPEND ${lib_prefix}_LIBS      RTIConnextDDS::c_api)
endif()

if (${PROJECT_NAME}_ENABLE_ARDUINO)
    list(APPEND ${lib_prefix}_LIBS      nano::nanoarduinoc)
endif()

if (DEFINED NANO_CORE_EXTRA_LIBS)
    list(APPEND ${lib_prefix}_LIBS     ${NANO_CORE_EXTRA_LIBS})
endif()

nano_component(${lib_prefix})
nano_component_library(${lib_prefix})
nano_component_install(${lib_prefix})

################################################################################
# "transportudpv4" component
################################################################################

set(NANO_TRANSPORT_UDPV4_INCLUDE_FILES_PUBLIC
        include/nano/nano_core_xrce_transport_udpv4.h)

set(NANO_TRANSPORT_UDPV4_SOURCE_FILES
        src/xrce/TransportUdp.c)

set(NANO_TRANSPORT_UDPV4_LIBS       nano::nanocore${agent_suffix})

set(NANO_TRANSPORT_UDPV4_LIBRARY    transportudpv4${agent_suffix})

if (WIN32)
    list(APPEND NANO_TRANSPORT_UDPV4_LIBS       netapi32)
endif()

if(${PROJECT_NAME}_ENABLE_TRANSPORT_UDPV4)
    nano_component(NANO_TRANSPORT_UDPV4)
    nano_component_library(NANO_TRANSPORT_UDPV4)
    nano_component_install(NANO_TRANSPORT_UDPV4)
endif()

################################################################################
# "transportserial" component
################################################################################

set(NANO_TRANSPORT_SERIAL_INCLUDE_FILES_PUBLIC
        include/nano/nano_core_xrce_transport_serial.h)

set(NANO_TRANSPORT_SERIAL_SOURCE_FILES
        src/xrce/TransportSerial.c)

set(NANO_TRANSPORT_SERIAL_LIBS      nano::nanocore${agent_suffix})

set(NANO_TRANSPORT_SERIAL_LIBRARY   transportserial${agent_suffix})

if(${PROJECT_NAME}_ENABLE_TRANSPORT_SERIAL)
    nano_component(NANO_TRANSPORT_SERIAL)
    nano_component_library(NANO_TRANSPORT_SERIAL)
    nano_component_install(NANO_TRANSPORT_SERIAL)
endif()

################################################################################

nano_project_install()

################################################################################

nano_project_summary()

################################################################################
