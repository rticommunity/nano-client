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

project(example-http-client C)

find_package(nano-client CONFIG REQUIRED COMPONENTS transportudpv4)

set(examples_http     http-client-udp)

foreach(example IN LISTS examples_http)
    add_executable(${example}           ${example}.c)
    if ("${example}" MATCHES "udp")
        set(nano_lib                    nano::nanoclientudpv4)
    elseif("${example}" MATCHES "serial")
        set(nano_lib                    nano::nanoclientserial)
    else()
        set(nano_lib                    nano::nanoclient)
    endif()
    target_link_libraries(${example}    ${nano_lib})
    install(TARGETS ${example} RUNTIME DESTINATION bin/examples)
endforeach()
