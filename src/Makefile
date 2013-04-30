OUTPUT_DIR = ../lib
INSTALL_PREFIX = /home/pi

TARGET = $(OUTPUT_DIR)/libDeltaPS.a

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h) 

INCLUDES = -I$(INSTALL_PREFIX)/include
LDFLAGS = -L$(INSTALL_PREFIX)/lib -lyat

OBJS = $(SOURCES:.cpp=.o)
RANLIB = ranlib

CXXFLAGS = -g $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJS) $(HEADERS) Makefile
	mkdir -p $(OUTPUT_DIR)
	$(AR) ru $@ $(OBJS)
	$(RANLIB) $@

install: $(TARGET)
	mkdir -p $(INSTALL_PREFIX)/{include,lib}
	install -m644 $(HEADERS) $(INSTALL_PREFIX)/include
	install -m644 $(TARGET) $(INSTALL_PREFIX)/lib

clean:
	rm -f $(TARGET) $(OBJS)
