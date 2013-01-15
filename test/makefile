#extension des fichiers à effacer lors du nettoyage
CLEANEXTS = o a

#fichier cible et répertoire d'installation
OUTPUTFILE = TestMagnet 
INSTALLDIR = ../bin


.PHONY: all
all: $(OUTPUTFILE)


TestMagnet : 	TestClass.o
			g++ -L../bin -lMagnet -o TestMagnet TestClass.o 	


TestClass.o : 	TestClass.cpp TestClass.h 
						g++ -c TestClass.cpp  -I../src


.PHONY: install
install:		
				mkdir -p $(INSTALLDIR)
				mv -f $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean
clean: 			
				for file in $(CLEANEXTS); do rm -f *.$$file; done

