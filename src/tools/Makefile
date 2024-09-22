# Copyright (c) 2016      Bryce Adelstein-Lelbach aka wash
# Copyright (c) 2000-2016 Paul Ullrich 
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying 
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Base directory.
TEMPESTTOOLSDIR= ../..

# Load Makefile framework. 
include $(TEMPESTTOOLSDIR)/mk/framework.make

TEMPESTTOOLSBASEDIR= $(TEMPESTTOOLSDIR)/src/base
TEMPESTTOOLSBASELIB= $(TEMPESTTOOLSBASEDIR)/libtoolsbase.a

TEMPESTTOOLSNETCDFDIR= $(TEMPESTTOOLSDIR)/src/netcdf-cxx-4.2
TEMPESTTOOLSNETCDFLIB= $(TEMPESTTOOLSNETCDFDIR)/libnetcdf_c++.a

EXEC_FILES= Test.cpp

EXEC_TARGETS= $(EXEC_FILES:%.cpp=%)

FILES= $(UTIL_FILES) $(EXEC_FILES)

.PHONY: all clean

# Build rules. 
all: $(EXEC_TARGETS)

$(EXEC_TARGETS): %: $(UTIL_FILES:%.cpp=$(BUILDDIR)/%.o) $(BUILDDIR)/%.o $(TEMPESTTOOLSBASELIB) $(TEMPESTTOOLSNETCDFLIB)
	$(CXX) $(LDFLAGS) -o $@ $(UTIL_FILES:%.cpp=$(BUILDDIR)/%.o) $(BUILDDIR)/$*.o $(TEMPESTTOOLSBASELIB) $(LIBRARIES)
	mv $@ $(TEMPESTTOOLSDIR)/bin

$(TEMPESTTOOLSBASELIB): %:
	cd $(TEMPESTTOOLSBASEDIR); $(MAKE)

$(TEMPESTTOOLSNETCDFLIB): %:
	cd $(TEMPESTTOOLSNETCDFDIR); $(MAKE)

# Clean rules.
clean:
	rm -rf $(DEPDIR)
	rm -rf $(BUILDDIR)

# Include dependencies.
-include $(FILES:%.cpp=$(DEPDIR)/%.d)

# DO NOT DELETE
