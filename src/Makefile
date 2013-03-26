OUTPUT_DIR = ../lib

TARGET = $(OUTPUT_DIR)/libDeltaPS.a

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)

OBJS = $(SOURCES:.cpp=.o)
RANLIB = ranlib

$(TARGET): $(OBJS) $(HEADERS) Makefile
	mkdir -p $(OUTPUT_DIR)
	$(AR) ru $@ $(OBJS)
	$(RANLIB) $@

clean:
	rm -f $(TARGET) $(OBJS)
