CXX ?= g++

YAML_CPP_PATH ?= ../yaml-cpp

CXXFLAGS += -g --std=c++11 -fPIC
CXXFLAGS += -Wall -Wno-deprecated-declarations
CXXFLAGS += -MMD
CXXFLAGS += $(shell pkg-config --cflags dbus-1 libcec;)

LDFLAGS += $(shell pkg-config --libs dbus-1 libcec;)
LDFLAGS += -rdynamic

GDB ?= gdb
SRC := $(wildcard *.cc)
OBJS := $(patsubst %.cc, %.o, $(SRC))
BIN ?= oas
DEPS = sh -c 'LIST1=" "; while [[ "$$LIST" != "$$LIST1" ]]; do \
	LIST="$$LIST1"; LIST1=`sed -n "s/^\#include \"\(.*\)\"$$/\1/p" $$0 $$LIST | sort -u | xargs`; \
	done; echo $$LIST;'

ENABLE_ASAN ?= false

ifeq ($(ENABLE_ASAN), true)
	CXXFLAGS += -fsanitize=address
endif

ENABLE_USAN ?= false
ifeq ($(ENABLE_USAN), true)
	CXXFLAGS += -fsanitize=undefined
endif

.PHONY: all clean run lines

-include Makefile.local

all: $(BIN)

run: $(BIN)
	./$<;

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@;

$(YAML_CPP_PATH)/libyaml-cpp.a:
	@[ -r $@ ] || ( echo Please make sure you have a static library at $@; exit 1 );

$(OBJS) : %.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@;

-include *.d

lines:
	wc -l *.cc *.h;

clean:
	@rm -fv *.o *.d $(BIN)
