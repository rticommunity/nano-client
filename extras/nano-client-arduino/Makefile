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

###############################################################################
# General directories
###############################################################################

BUILD_DIR 			?= $(shell pwd)/build
INSTALL_DIR			?= $(shell pwd)/install

###############################################################################
# Project directories
###############################################################################
SRC_DIR				?= src
RESOURCE_DIR		?= resource
PROPERTIES_DIR		?= $(RESOURCE_DIR)/arduino/properties
PLATFORMIO_DIR		?= $(RESOURCE_DIR)/platformio
XML_DIR				?= $(RESOURCE_DIR)/xml
XML_EXAMPLES_DIR	?= $(XML_DIR)/examples
EXAMPLES_DIR		?= examples
LIB_PREFIX			?= 

###############################################################################
# Generated archive configuration
###############################################################################
ARCHIVE_PREFIX		?= nano-arduino
ARCHIVE_VERSION     ?= $(shell $(GIT_SUFFIX))
ARCHIVE_FILE        ?= $(ARCHIVE_PREFIX)-$(ARCHIVE_VERSION)
ARCHIVE_PATH        ?= .
ARCHIVE_FORMATS     ?= zip \
					   tgz
ARCHIVE             ?= $(ARCHIVE_PATH)/$(ARCHIVE_FILE)
ARCHIVES            ?= $(ARCHIVE_FORMATS:%=$(ARCHIVE).%)

###############################################################################
# NanoClient library
###############################################################################
nano-client-arduino_DIR	:= $(LIB_PREFIX)nano-client-arduino
nano-client-arduino_SRC	:= nano_client_arduino.h

nano-client-arduino_EXAMPLES := SensorWriterUdp \
						   SensorWriterSerial \
						   SensorWriterRefUdp \
						   SensorWriterRefSerial \
						   SensorReaderRefUdp \
						   SensorReaderSerial \
						   SensorReaderUdp \
						   ShapeReaderUdp \
						   ShapeWriterUdp

nano-client-arduino_EXTRAS := $(XML_EXAMPLES_DIR)/sensor_agent.xml \
						   $(XML_EXAMPLES_DIR)/shape_agent.xml

###############################################################################
# Generated Arduino libraries
###############################################################################
LIBRARIES := nano-client-arduino

LIBRARY_DIRS	?= $(foreach lib, $(LIBRARIES), $(call LIB_REL_DIR,$(lib)))

###############################################################################
# Load nano's build configuration using the utility script FindNano.mk
###############################################################################
export NANO_DIR			?= $(shell pwd)/../nano-client-cpp
export NANO_BUILD_DIR	?= $(BUILD_DIR)/nano-client-cpp-build
export NANO_INSTALL_DIR	?= $(BUILD_DIR)/nano-client-cpp-install

NANO_MK					?= $(shell pwd)/resource/make/nano_source_helper.mk

###############################################################################
# Make target declarations
###############################################################################
CMAKE				?= cmake

ifneq ($(VERBOSE),)
	CP				?= cp -v
	MKDIR			?= mkdir -v
	ZIP				?= zip
	TAR				?= tar -v
	RM				?= rm -v
else
	CP				?= cp
	MKDIR			?= mkdir
	ZIP				?= zip -q
	TAR				?= tar
	RM				?= rm
	SUPPRESS_INFO	:= >/dev/null
endif

define TASK_COMPLETE
	printf "[%s] %s OK\n" "$(strip $(1))" "$(strip $(2))" # $(SUPPRESS_INFO)
endef

define GENERATED_ARCHIVE
	printf "Generated archive: %s\n" "$(strip $(1))"
endef

define GIT_SUFFIX
	git symbolic-ref -q --short HEAD || git describe --tags --exact-match
endef

define LIB_REL_DIR
$($(strip $(1))_DIR)
endef

define LIB_DIR
$(INSTALL_DIR)/$(call LIB_REL_DIR,$*)
endef

define LIB_SRC
$($(strip $(1))_SRC:%=$(SRC_DIR)/%)
endef

define LIB_EXAMPLES
$($(strip $(1))_EXAMPLES:%=$(EXAMPLES_DIR)/%)
endef

define LIB_EXTRAS
$($(strip $(1))_EXTRAS)
endef

define LIB_DIR_EXTRAS
$(call LIB_DIR, $(1))/extras
endef

define LIB_DIR_EXAMPLES
$(call LIB_DIR, $(1))/examples
endef

define LIB_DIR_SRC
$(call LIB_DIR, $(1))/src
endef

define NANO_CFG
$(SRC_DIR)/$(strip $(1))_nano_core_user_config.h
endef

define LIB_VERSION
$(shell grep '^version' $(call LIB_DIR,$(1))/library.properties | cut -d'=' -f2)
endef

define LIB_ARCHIVE_FILE
$($(strip $(1))_DIR)-$(call LIB_VERSION,$(1))
endef


.PHONY: \
	all \
	install \
	clean \
	clean-install \
	fullarchive \
	archive \
	nano-agent \
	nano-agent-clean

all: install

install: $(LIBRARIES:%=%.library)
	@echo Arduino libraries installed: $(INSTALL_DIR)

clean: $(LIBRARIES:%=%.clean)
	@$(RM) -rf $(NANO_INSTALL_DIR) $(NANO_BUILD_DIR)

clean-install: clean \
			   install

fullarchive: $(ARCHIVES:%=%.fullarchive)

%.zip.fullarchive: install
	@cd $(INSTALL_DIR) && \
	$(RM) -f $*.zip && \
	$(ZIP) -r $*.zip $(call LIBRARY_DIRS)
	@$(call GENERATED_ARCHIVE, $*.zip)

%.tgz.fullarchive: install
	@cd $(INSTALL_DIR) && \
	$(RM) -f $*.tgz && \
	$(TAR) -czf $*.tgz $(call LIBRARY_DIRS)
	@$(call GENERATED_ARCHIVE, $*.tgz)

archive: $(foreach tgt,$(ARCHIVE_FORMATS:%=%.archive),$(LIBRARIES:%=%.$(tgt)))

%.zip.archive: %.library
	@cd $(INSTALL_DIR) && \
	    lib_archive="$(call LIB_ARCHIVE_FILE,$*).zip" && \
	    $(RM) -f $${lib_archive} && \
	    $(ZIP) -r $${lib_archive} $($*_DIR) && \
	    $(call GENERATED_ARCHIVE, $${lib_archive})

%.tgz.archive: %.library
	@cd $(INSTALL_DIR) && \
	    lib_archive="$(call LIB_ARCHIVE_FILE,$*).tgz" && \
	    $(RM) -f $${lib_archive} && \
		$(TAR) -czf $${lib_archive} $($*_DIR) && \
	    $(call GENERATED_ARCHIVE, $${lib_archive})

%.archive : 

nano-client-arduino.dist: %.dist : %.nano

%.library: %.init \
		   %.properties \
		   %.platformio \
		   %.src \
		   %.extras \
		   %.examples \
		   %.dist \
		   %.cleanempty
	@$(call TASK_COMPLETE, $*, library)

%.init:
	@echo Bundling library $*

%.properties:
	@$(MKDIR) -p $(call LIB_DIR, $*)
	@$(CP) $(PROPERTIES_DIR)/$*.properties \
	       $(call LIB_DIR, $*)/library.properties
	@$(call TASK_COMPLETE, $*, properties)

%.platformio:
	@$(MKDIR) -p $(call LIB_DIR, $*)
	@$(CP) $(PLATFORMIO_DIR)/$*-library.json \
	       $(call LIB_DIR, $*)/library.json
	@$(call TASK_COMPLETE, $*, platform-io)

%.src:
	@$(MKDIR) -p $(call LIB_DIR_SRC, $*)
	@$(CP) $(call LIB_SRC, $*) \
		   $(call LIB_DIR_SRC, $*)
	@$(call TASK_COMPLETE, $*, src)

%.extras:
	@$(MKDIR) -p $(call LIB_DIR_EXTRAS, $*)
	@if [ -n "$($*_EXTRAS)" ]; then \
	    $(CP) -r $(call LIB_EXTRAS, $*) \
	             $(call LIB_DIR_EXTRAS, $*) ;\
	fi
	@$(call TASK_COMPLETE, $*, extras)

%.examples:
	@$(MKDIR) -p $(call LIB_DIR_EXAMPLES, $*)
	@if [ -n "$($*_EXAMPLES)" ]; then \
	    $(CP) -r $(call LIB_EXAMPLES, $*) \
	             $(call LIB_DIR_EXAMPLES, $*) ;\
	fi
	@$(call TASK_COMPLETE, $*, examples)

%.dist:
	@$(call TASK_COMPLETE, $*, dist)

%.cleanempty:
	@for d in $$(find $(call LIB_DIR, $*) -empty -type d); do \
	    $(RM) -rf $${d} ;\
	done

%.nano : %.nanosrc
	@$(call TASK_COMPLETE, $*, nano)

%.clean :
	@echo Cleaning install directory $(call LIB_DIR, $*)
	@$(RM) -rf $(call LIB_DIR, $*)

%.nanosrc: $(NANO_INSTALL_DIR)
	@$(MAKE) -f $(NANO_MK) \
	        NANO_SRC_PREFIX=$(call LIB_DIR_SRC, $*)/nano-src \
			NANO_INC_PREFIX=$(call LIB_DIR_SRC, $*) \
	        copy-src
	@$(call TASK_COMPLETE, $*, nano-src)

$(NANO_INSTALL_DIR):
	@$(MAKE) -f $(NANO_MK) gen-src
