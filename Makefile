# Get the current OS and architecture
OS ?= $(shell uname -s)
CPU ?= $(shell uname -m)
PLATFORM ?= $(OS).$(CPU)

default: all

include platforms/$(PLATFORM).mk

INCLUDES = $(wildcard include/*.hpp) $(wildcard include/*/*.hpp) $(wildcard include/*/*/*.hpp)

all: sample.$(SHLIB_SUFFIX)

clean:
	rm -f sample.$(SHLIB_SUFFIX)

sample.$(SHLIB_SUFFIX): sample.cpp $(INCLUDES)
	$(CXXLIB) -Iinclude -o sample.$(SHLIB_SUFFIX) sample.cpp -lpthread
