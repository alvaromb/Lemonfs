#include "bloques.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "directorios.h"

#include <stdlib.h>

int main(int argc, char **argv) {
	
	if (argc == 3) {
		
		bmount(argv[1]);
		
		int nbloques = atoi(argv[2]);
		if (nbloques < 0) {
			nbloques = nbloques*(-1);
		}
		
		unsigned char buffer[TB];
		int i;
		for (i = 0; i<nbloques; i++) {
			bwrite(i, buffer);
		}
		
		initSB(nbloques, argv[1]);
		initMB(nbloques);
		initAI(nbloques);
		
		leerSB();
		
		bumount();
	}
	
	return (0);
}
