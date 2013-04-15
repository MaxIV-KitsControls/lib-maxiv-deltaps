OUTPUT_DIR = ../lib
INSTALL_PREFIX = /home/tango

TARGET = $(OUTPUT_DIR)/TestMagnet

SOURCES = pstest.cpp
HEADERS = 

CXXFLAGS = -I../../src
LDFLAGS = -L../../lib -lDeltaPS -lyat  

OBJS = $(SOURCES:.cpp=.o)
RANLIB = ranlib

all: $(TARGET)

$(TARGET): $(OBJS) $(HEADERS) Makefile
	mkdir -p $(OUTPUT_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

install: $(TARGET)
	mkdir -p $(INSTALL_PREFIX)/{include,lib}
	install -m644 $(HEADERS) $(INSTALL_PREFIX)/include
	install -m644 $(TARGET) $(INSTALL_PREFIX)/lib

clean:
	rm -f $(TARGET) $(OBJS)
