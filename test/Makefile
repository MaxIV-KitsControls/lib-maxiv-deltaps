TARGET = pstest

CXXFLAGS = -I../src
LDFLAGS = -L../lib -lDeltaPS -lyat  

all: $(TARGET)

$(TARGET): $(TARGET).o Makefile 
	$(MAKE) -C ../src
	$(CXX) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
