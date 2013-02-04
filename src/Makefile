OUTPUT_DIR = ../lib
INSTALL_PREFIX = /home/tango

TARGET = $(OUTPUT_DIR)/libMagnet.a

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

OBJS = $(SOURCES:.cpp=.o)
RANLIB = ranlib

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
