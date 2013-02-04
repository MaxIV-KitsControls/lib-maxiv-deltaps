#extension des fichiers à effacer lors du nettoyage
CLEANEXTS = o a

#fichier cible et répertoire d'installation
OUTPUTFILE = libMagnet.a
INSTALLDIR = ../lib


.PHONY: all
all: $(OUTPUTFILE)


libMagnet.a : 	ClassMagnet.o Exception.o
				ar ru libMagnet.a ClassMagnet.o Exception.o
				ranlib libMagnet.a


ClassMagnetTest.o : 	ClassMagnet.cpp ClassMagnet.h 
						g++ -c ClassMagnet.cpp

Exception.o : 	Exception.cpp Exception.h
				g++ -c Exception.cpp


.PHONY: install
install:		
				mkdir -p $(INSTALLDIR)
				mv $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean: 			
				for file in $(CLEANEXTS); do rm -f *.$$file; done

ClassMagnet.o: ClassMagnet.h Exception.h
Exception.o: Exception.h


