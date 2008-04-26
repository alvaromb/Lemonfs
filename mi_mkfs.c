#include "bloques.h"
#include "ficheros_basico.h"

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
		
		leerSB();
		
		/* IMPRIMIMOS EL MAPA DE BITS */
		bread(1, buffer);
		for (i = 0; i < 30; i++) {
			printf("%d\n", buffer[i%TB]);
		}
		
		
		bumount();
	}
	
	return (0);
}
