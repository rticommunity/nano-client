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

CMAKE			?= cmake
BUILD_DIR		?= $(shell pwd)/build
INSTALL_DIR		?= $(shell pwd)/install
BUILD_TYPE		?= Release
BUILD_SHARED	?= ON
CMAKE_GENERATOR ?= Unix Makefiles
ENABLE_EXAMPLES ?= OFF
ENABLE_TESTS    ?= OFF

.PHONY : all \
		 build \
		 cmake \
		 install \
		 clean \
		 clean-deep

all: build

install: build
	$(CMAKE) --build $(BUILD_DIR) --target install -- -j4

build: cmake
	$(CMAKE) --build $(BUILD_DIR) -- -j4

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean

clean-deep:
	rm -rf $(BUILD_DIR) $(INSTALL_DIR)

cmake: $(BUILD_DIR).dir
	$(CMAKE) -B$(BUILD_DIR) -H. -G "$(CMAKE_GENERATOR)" \
	                            -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
	                            -DCMAKE_INSTALL_PREFIX=$(INSTALL_DIR) \
	                            -DBUILD_SHARED_LIBS=$(BUILD_SHARED) \
	                            -DENABLE_EXAMPLES=$(ENABLE_EXAMPLES) \
	                            -DENABLE_TESTS=$(ENABLE_TESTS) $(CMAKE_ARGS)

%.dir:
	mkdir -p $*
