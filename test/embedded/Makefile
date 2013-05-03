TARGET = pstest
PLATFORM = "pi"

OBJS = $(TARGET).o

CXXFLAGS = -I../../src -I/home/pi/include -g
LDFLAGS = -L../../lib -lDeltaPS -L/home/pi/lib -lyat -L/lib/arm-linux-gnueabihf -lpthread -ldl
CFLAGS = -g

ifeq ($(PLATFORM), "pi")
OBJS += bcm2835_gpio.o
endif

ifeq ($(PLATFORM), "beagle")
OBJS += am335x_gpio.o
endif

all:	$(TARGET)

$(TARGET): $(OBJS) Makefile 
	$(MAKE) -C ../../src
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

clean:
	$(MAKE) -C ../../src clean
	rm -f $(TARGET) $(OBJS)
