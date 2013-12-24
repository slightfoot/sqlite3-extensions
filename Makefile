#
# Builds all $DIRS as SQLite3 Extensions!
#
DIRS = sqlite3_base64_ext sqlite3_hex_ext sqlite3_pad_ext

export SQLITE3_EXTENSIONS_SRC = ../sqlite_extensions/sqlite_extensions.cpp
export SQLITE3_EXTENSIONS_INC = -I../sqlite_extensions/ -I../sqlite-amalgamation-3080200/

export CXX = g++
export CXXFLAGS = -Ofast -static -shared -lstdc++ \
	-fdata-sections -ffunction-sections \
	-Wl,--gc-sections
#-fPIC


BUILDDIRS   = $(DIRS:%=build-%)
INSTALLDIRS = $(DIRS:%=install-%)
CLEANDIRS   = $(DIRS:%=clean-%)
TESTDIRS    = $(DIRS:%=test-%)

all: $(BUILDDIRS)
$(DIRS): $(BUILDDIRS)
$(BUILDDIRS):
	@$(MAKE) --no-print-directory -C $(@:build-%=%)

install: all $(INSTALLDIRS)
$(INSTALLDIRS):
	@$(MAKE) --no-print-directory -C $(@:install-%=%) install

test: all $(TESTDIRS)
$(TESTDIRS): 
	@$(MAKE) --no-print-directory -C $(@:test-%=%) test

clean: $(CLEANDIRS)
$(CLEANDIRS): 
	@$(MAKE) --no-print-directory -C $(@:clean-%=%) clean

.PHONY: subdirs $(DIRS)
.PHONY: subdirs $(BUILDDIRS)
.PHONY: subdirs $(INSTALLDIRS)
.PHONY: subdirs $(TESTDIRS)
.PHONY: subdirs $(CLEANDIRS)
.PHONY: all install clean test
