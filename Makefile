##  @file   Makefile
##  @brief  This file is part of the "maze" project ; it mainly contains build
##          directives to generate the project's executable.
##
##  @par References :
##  - [https://stackoverflow.com/a/2484343]

#
# Generic variables definition
#

# To easily "echo" messages in color
TERM_COL_BLU=\033[44m
TERM_COL_GRN=\033[30;42m
TERM_COL_INV=\033[7;40m
TERM_COL_ORG=\033[0;43m
TERM_COL_RED=\033[0;41m
TERM_COL_STD=\033[0m

# To easily format "echo" messages.
TERM_FMT_BOLD=`tput bold`
TERM_FMT_STD=`tput sgr0`



#
# (re-)Define command aliases
#
CC=gcc
ECHO=echo
LD=gcc
MKDIR=mkdir -p

define RM
	@$(ECHO)    "$(TERM_COL_ORG)    $(TERM_FMT_BOLD)RM  $1$(TERM_FMT_STD)"
	@rm -rvf $1
endef



#
# Define project-related variables
#

##  @brief  Defines the name of the application ; used for the executable's name
APP_NAME=maze

##  @brief  Defines the directory in which generated objects will be placed.
DIR_BUILD=build

##  @brief  Defines the directory in which the generated software will be
##          placed.
DIR_OUTPUT=out



#
# Define sources and objects paths
#
MODULES		:= . #main test
SRC_DIR		:= $(addprefix src/,$(MODULES))
BUILD_SUBDIRS	:= $(addprefix $(DIR_BUILD)/,$(MODULES))

SRC		:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ		:= $(patsubst src/%.c,$(DIR_BUILD)/%.o,$(SRC))
INCLUDES	:= $(addprefix -I,$(SRC_DIR))

# for details, see :
# https://www.gnu.org/software/make/manual/html_node/General-Search.html
vpath %.c $(SRC_DIR)



#
# Create a "canned command" which will be used to generate objects.
# Doing this avoids copy & paste of the same command in many lines, and
# if some command argument changes, we modify them here instead of everywhere
# in the makefile.
# We could also have used $(CFLAGS) and other related variables, but this
# method seems quite more readable for a small project like this one where
# all objects can have the same build command.
#
define make-goal
$1/%.o: %.c
	@$(ECHO) -n	"$(TERM_COL_INV)    $(TERM_FMT_BOLD)CC  $$<$(TERM_FMT_STD)"
	@$(ECHO) 	"  -->  $$@$(TERM_COL_STD)"
	@$(CC) $(INCLUDES) -c $$< -o $$@
endef



# ##############################################################################
# ##############################################################################
#   Default target definition
# ##############################################################################

#
# Force re-execution of some makefile targets
#
.PHONY: all checkdirs clean



#
# Default target
# This first target will be the one called by default.
#
default: all



# ##############################################################################
# ##############################################################################
#   Targets definitions to install dependencies
# ##############################################################################
install-dependencies:
	apt install -qq libncurses5 libncurses5-dev



# ##############################################################################
# ##############################################################################
#   Targets definitions to Build
# ##############################################################################

#
# This is the main target ; it will produce the main executable.
#
all: checkdirs $(DIR_OUTPUT)/$(APP_NAME)



#
# Dummy target to verify all needed directories have been created.
#
checkdirs: $(BUILD_SUBDIRS) $(DIR_OUTPUT)



#
# This target creates needed directories.
#
$(BUILD_SUBDIRS) $(DIR_OUTPUT):
	@$(ECHO)	"$(TERM_COL_ORG)    $(TERM_FMT_BOLD)RM  $@$(TERM_FMT_STD)"
	@$(MKDIR) $@



#
# Main application's target.
#
$(DIR_OUTPUT)/$(APP_NAME): $(OBJ)
	@$(ECHO) 	"$(TERM_COL_INV)    $(TERM_FMT_BOLD)LD  $@$(TERM_FMT_STD)"
	@$(LD) $^ -o $@
	@$(ECHO)	"$(TERM_COL_GRN)    $(TERM_FMT_BOLD)--> $@ generated successfully !$(TERM_FMT_STD)"



#
# Produce build directives for submodules by expanding the "canned command"
# for each submodule.
#
$(foreach bdir,$(BUILD_SUBDIRS),$(eval $(call make-goal,$(bdir))))



# ##############################################################################
# ##############################################################################
#   Targets definitions to Clean
# ##############################################################################

#
# This target remove auto-generated objects from the project tree ; it will NOT
# remove the generated executable.
#
clean:
	$(call RM,$(DIR_BUILD))



#
# This target removes the build directory AND the generated executable.
# It shall leave a project tree containing only versionned files.
#
distclean: clean
	$(call RM,$(DIR_OUTPUT)/$(APP_NAME))

