
ifndef SC3_SRC
$(error "You must set environment variable SC3_SRC")
else ifndef LINK_SRC
$(error "You must set environment variable LINK_SRC")
endif


UNAME :=$(shell uname)

ifeq ($(UNAME), Darwin)
CXXLDFLAGS=-bundle
TARGET=Link.scx
PLATFORM=LINK_PLATFORM_MACOSX

else ifeq ($(UNAME), Linux)
TARGET=Link.so
CXXLDFLAGS=-shared
PLATFORM=LINK_PLATFORM_LINUX
endif


OBJS=Link.o

CXXFLAGS=-I$(SC3_SRC)/include/plugin_interface/ -I$(SC3_SRC)/include/common -I$(LINK_SRC)/include/ -I$(LINK_SRC)/modules/asio-standalone/asio/include -std=c++11 -D$(PLATFORM)

all: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) $(CXXLDFLAGS) -o$(TARGET) $(OBJS)
	mv $(TARGET) ./LinkUGen

clean :
	rm ./LinkUGen/$(TARGET) $(OBJS)
