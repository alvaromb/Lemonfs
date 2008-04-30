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
		
		/* INICIO PRUEBAS */
				
		/* IMPRIMIMOS EL MAPA DE BITS 
		bread(1, buffer);
		for (i = 0; i < TB; i++) {
			printf("%d ", buffer[i]);
			if ((i != 0) && ((i % 20) == 0)) {
				printf("\n");
			}
		}*/
		/* IMPRIMIMOS LOS INODOS 
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
		}*/
		
		/* PRUEBAS CON ESCRITURA-LECTURA-RESERVA DE BLOQUES */
		/*int rb;
		int j;
		int puta;
		for (j = 0; j < 16000; j++) {
			rb = reservar_bloque();
			if (i == 1) {
				puta = rb;
			}
			printf("rb: %d\n", rb);
			//sleep(1);
		}
		printf("PUTA: %d\n", puta);*/
		
		/* PRUEBAS CON INODOS */
		struct superbloque SB;
		bread(0, (char *)&SB);
		struct inodo in;
		int perra;
		int j;
		for (j = 0; j < SB.i_libres; j++) {
			perra = reservar_inodo(in);
			printf("perra: %d\n", perra);
		}
		
		
			
		/* FIN PRUEBAS */
		
		
		leerSB();
		
		bumount();
	}
	
	return (0);
}
