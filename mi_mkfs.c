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
		initAI(nbloques);
		
		leerSB();
		
		/* IMPRIMIMOS EL MAPA DE BITS
		bread(1, buffer);
		for (i = 0; i < 30; i++) {
			printf("%d\n", buffer[i%TB]);
		}*/
		/* IMPRIMIMOS LOS INODOS */
		struct inodo in;
		int j;
		for (j = 2; j < tamAI(nbloques)+3; j++) {
			bread(j, buffer);
			for (i = 0; i < 16; i++) {
				if (in.pb_ind[0] == 0) {
					break;
				}
				memcpy(&in, &buffer[(i%16)*64], sizeof(struct inodo));
				printf("inodo: %d, siguiente: %d\n", i, in.pb_ind[0]);
			}			
		}
		
		/* FIN PRUEBAS */
		
		
		bumount();
	}
	
	return (0);
}
