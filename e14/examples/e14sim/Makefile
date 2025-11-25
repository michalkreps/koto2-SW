# -*- Makefile -*-

MAINS = e14sim
PACKAGE = $(shell basename `pwd`)
INSTALL_LIBS = no
INSTALL_BINS = yes

include $(E14_CONFIG_DIR)/Makefile.g4

pre::
ifeq "$(shell uname)" "Darwin"
	@cp REQUIREMENTS REQUIREMENTS.orig && sed "/history/d" REQUIREMENTS.orig > REQUIREMENTS
else
	@if ! grep -q history REQUIREMENTS; then echo "history" >> REQUIREMENTS;fi
endif
	echo "Converting sources..."
	@sh convertSource.sh