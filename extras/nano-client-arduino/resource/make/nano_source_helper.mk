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

CMAKE 				?= cmake

ifneq ($(VERBOSE),)
	CP				?= cp -v
	MKDIR			?= mkdir -v
else
	CP				?= cp
	MKDIR			?= mkdir
	SUPPRESS_INFO	:= >/dev/null
endif

-include $(NANO_INSTALL_DIR)/src/makefile

nano_SOURCE_FILES	:= $(nano_core_core_SOURCE_FILES) \
					   $(nano_core_transportudpv4_SOURCE_FILES) \
					   $(nano_core_transportserial_SOURCE_FILES) \
					   $(nano_client_client_SOURCE_FILES) \
					   $(nano_client_cpp_cppclient_SOURCE_FILES) \
					   $(nano_client_cpp_cpptransportudpv4_SOURCE_FILES) \
					   $(nano_client_cpp_cpptransportserial_SOURCE_FILES) \
					   $(nano_arduino_c_arduinoc_SOURCE_FILES)

nano_HEADER_FILES	:= $(nano_core_core_INC_PUB_FILES) \
					   $(nano_core_transportudpv4_INC_PUB_FILES) \
					   $(nano_core_transportserial_INC_PUB_FILES) \
					   $(nano_client_client_INC_PUB_FILES) \
					   $(nano_client_cpp_cppclient_INC_PUB_FILES) \
					   $(nano_client_cpp_cpptransportudpv4_INC_PUB_FILES) \
					   $(nano_client_cpp_cpptransportserial_INC_PUB_FILES) \
					   $(nano_arduino_c_arduinoc_INC_PUB_FILES)

NANO_INC_PREFIX		?= $(NANO_SRC_PREFIX)/include

nano_SOURCE_DIR		:= $(NANO_SRC_PREFIX)/
nano_HEADER_DIR		:= $(NANO_INC_PREFIX)/nano

.PHONY: copy-src \
        copy-user-cfg \
        gen-src

copy-src: $(nano_SOURCE_FILES) \
          $(nano_HEADER_FILES)
	@$(MKDIR) -p $(nano_SOURCE_DIR) \
	       		 $(nano_HEADER_DIR)
	@$(CP) $(nano_SOURCE_FILES) $(nano_SOURCE_DIR)/
	@$(CP) $(nano_HEADER_FILES) $(nano_HEADER_DIR)/

copy-user-cfg:
	@$(MKDIR) -p $(nano_HEADER_DIR)
	@$(CP) $(NANO_USER_CFG)	$(nano_HEADER_DIR)/nano_core_user_config.h

gen-src: $(nano_SOURCE_FILES) \
         $(nano_HEADER_FILES)
	@rm -rf $(NANO_BUILD_DIR)
	@mkdir -p $(NANO_BUILD_DIR)
	@cd $(NANO_BUILD_DIR) && \
		$(CMAKE) $(NANO_DIR) \
	                         -DCMAKE_INSTALL_PREFIX=$(NANO_INSTALL_DIR) \
	                         -DBUILD_LIBRARIES=OFF \
							 -DBUILD_EXECUTABLES=OFF \
	                         -DINSTALL_SOURCE=ON \
	                         -DENABLE_ARDUINO=ON $(SUPPRESS_INFO)
	@cmake --build $(NANO_BUILD_DIR) --target install -- -j4 $(SUPPRESS_INFO)
