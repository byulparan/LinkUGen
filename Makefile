
ifndef SC3_SRC
$(error "You must set environment variable SC3_SRC")
else ifndef LINK_SRC
$(error "You must set environment variable LINK_SRC")
endif


UNAME :=$(shell uname)
ifeq ($(UNAME), Darwin)
CXX=clang++
CXXLDFLAGS=-bundle
else ifeq ($(UNAME), Linux)
CXX=g++
CXXLDFLAGS=-shared
endif

TARGET=Link.scx
OBJS=Link.o
CXXFLAGS=-I$(SC3_SRC)/include/plugin_interface/ -I$(SC3_SRC)/include/common -I$(LINK_SRC)/include/ -I$(LINK_SRC)/modules/asio-standalone/asio/include -std=c++1y -DLINK_PLATFORM_MACOSX

all: $(TARGET)

$(TARGET) : $(OBJS)
	clang++ $(CXXLDFLAGS) -o$(TARGET) $(OBJS)
	mv $(TARGET) ./LinkUGen

clean :
	rm ./LinkUGen/$(TARGET) $(OBJS)
