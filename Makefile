#
# This software was developed at the National Institute of Standards and
# Technology (NIST) by employees of the Federal Government in the course
# of their official duties. Pursuant to title 17 Section 105 of the
# United States Code, this software is not subject to copyright protection
# and is in the public domain. NIST assumes no responsibility  whatsoever for
# its use by other parties, and makes no guarantees, expressed or implied,
# about its quality, reliability, or any other characteristic.
#
#
# Top-level make file to build all of the EVALUATION common code.
#
include common.mk
SRCDIRS := nbis src
INSDIRS := src

LOCALINC := $(PWD)/src/include
LOCALLIB := $(PWD)/lib

all:
	test -d $(LOCALLIB) || mkdir $(LOCALLIB)
	@for subdir in $(SRCDIRS); do \
		(cd $$subdir && $(MAKE) all) || exit 1; \
	done

install:
	@for subdir in $(INSDIRS); do \
		(cd $$subdir && $(MAKE) install) || exit 1; \
	done

clean:
	@for subdir in $(SRCDIRS); do \
		(cd $$subdir && $(MAKE) clean) || exit 1; \
	done
	rm -rf $(LOCALLIB)
	rm -f .gdb_history
