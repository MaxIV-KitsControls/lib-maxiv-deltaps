#extension des fichiers à effacer lors du nettoyage
CLEANEXTS = o a
TARGET = pstest

#fichier cible et répertoire d'installation
OUTPUTFILE = TestMagnet 
INSTALLDIR = ../bin
CXXFLAGS = -I../src
LDFLAGS = -L../lib -lDeltaPS -lyat  

all: $(TARGET)

.PHONY: all
all: $(OUTPUTFILE)


TestMagnet : 	TestClass.o
			g++ -D_REENTRANT -o TestMagnet TestClass.o -L../lib -lMagnet -lpthread

TestClass.o : 	TestClass.cpp TestClass.h 
						g++ -c TestClass.cpp  -I../src


.PHONY: install
install:		
				mkdir -p $(INSTALLDIR)
				mv -f $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean: 			
				for file in $(CLEANEXTS); do rm -f *.$$file; done
$(TARGET): $(TARGET).o Makefile 
	$(MAKE) -C ../src
	$(CXX) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)
