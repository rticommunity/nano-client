
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
SRC_DIR := $(patsubst %/,%,$(dir $(mkfile_path)))

SOURCE_DIRECTORIES = $(filter-out %/makefile, $(wildcard $(SRC_DIR)/*))
SOURCE_DIRECTORIES_TARGETS = $(notdir $(SOURCE_DIRECTORIES))

.PHONY: src

src : $(SOURCE_DIRECTORIES_TARGETS)

$(foreach src_dir, $(SOURCE_DIRECTORIES),\
$(eval include $(src_dir)/makefile))
