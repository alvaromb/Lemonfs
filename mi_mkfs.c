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
		/*struct superbloque SB;
		bread(0, (char *)&SB);
		struct inodo in;
		int perra;
		int j;
		for (j = 0; j < SB.i_libres; j++) {
			perra = reservar_inodo(in);
			printf("perra: %d\n", perra);
		}*/
		
		/* PRUEBAS CON LIBERAR_BLOQUES E INODOS 
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 20; 
		int pp;
		for (pp = 0; pp < TAM_PIND; pp++) {
			in.pb_ind[pp] = 0;
		}
		
		leerSB();*/
		
		/* Punteros directos 
		unsigned int bufferp[TP];
		int j;
		for (j = 0; j < TAM_PDIR; j++) {
			in.pb_dir[j] = reservar_bloque();
		}
		printf("Punteros directos listos\n");
		
		leerSB();
		//sleep(15);*/
		
		/* Puntero indirecto 0 
		in.pb_ind[0] = reservar_bloque();
		for (j = 0; j < TP; j++) {
			bufferp[j] = reservar_bloque();
		}
		bwrite(in.pb_ind[0], bufferp);
		printf("Puntero indirecto 0 listo\n");
		
		leerSB();
		//sleep(15);*/
		
		/* Puntero indirecto 1 
		unsigned int bufferpp[TP];
		in.pb_ind[1] = reservar_bloque();
		for (j = 0; j < TP; j++) {
							
			int k;
			for (k = 0; k < TP; k++) {
				bufferpp[k] = reservar_bloque();
			}
			bufferp[j] = reservar_bloque();
			bwrite(bufferp[j], bufferpp);
		}
		bwrite(in.pb_ind[1], bufferp);
		printf("Puntero indirecto 1 listo\n");
		
		leerSB();
		//sleep(15);*/
		
		/* Puntero indirecto 2 
		unsigned int bufferppp[TP];
		in.pb_ind[2] = reservar_bloque();
		for (j = 0; j < TP; j++) {
			int k;
			for (k = 0; k < TP; k++) {
					int l;
					for (l = 0; l < TP; l++) {
						bufferppp[l] = reservar_bloque();
					}
					bufferpp[k] = reservar_bloque();
					bwrite(bufferpp[k], bufferppp);
			}
			bufferp[j] = reservar_bloque();
			bwrite(bufferp[j], bufferpp);
		}
		bwrite(in.pb_ind[2], bufferp);*/
		
		//int perra = reservar_inodo(in);
		//printf("PERRA: %d\n\n", perra);
		
		//liberar_inodo(0);
		//liberar_bloques(0, 532480);
		
		/* Leemos el inodo 
		leer_inodo(&in, 0);
		int k;
		for (k = 0; k < 3 ; k++) {
			printf("dir: %d\n", in.pb_dir[k]);
		}
		for (k = 0; k < TAM_PIND; k++) {
			printf("ind: %d\n", in.pb_ind[k]);
		}*/
		
		
		/* PRUEBAS CON TRADUCIR_BLOQUE_INODO 
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 0; 
		int pp;
		for (pp = 0; pp < TAM_PIND; pp++) {
			in.pb_ind[pp] = 0;
		}
		for (pp = 0; pp < TAM_PDIR; pp++) {
			in.pb_dir[pp] = 0;
		}
		
		
		printf("inodo = %d\n\n", reservar_inodo(in));
		
		int perra = traducir_bloque_inodo(0, 264, 1);
		printf("perra = %d\n", perra);
		int perra2 = traducir_bloque_inodo(0, 264, 0);
		printf("perra2 = %d\n", perra2);
		perra = traducir_bloque_inodo(0, 265, 1);
		printf("perra3 = %d\n", perra);
		perra = traducir_bloque_inodo(0, 263, 1);
		printf("perra4 = %d\n", perra); */
		
		
		/* PRUEBAS CON mi_write_f 
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 0; 
		int pp;
		for (pp = 0; pp < TAM_PIND; pp++) {
			in.pb_ind[pp] = 0;
		}
		for (pp = 0; pp < TAM_PDIR; pp++) {
			in.pb_dir[pp] = 0;
		}
		
		printf("inodo = %d\n\n", reservar_inodo(in));
				
		const void *buf;
		buf = malloc(sizeof(8000));
		mi_write_f(0, buf, 67379200, 8000); */
		
		
		/* PRUEBAS CON mi_read_f 
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 0; 
		int pp;
		for (pp = 0; pp < TAM_PIND; pp++) {
			in.pb_ind[pp] = 0;
		}
		for (pp = 0; pp < TAM_PDIR; pp++) {
			in.pb_dir[pp] = 0;
		}
		
		printf("inodo = %d\n\n", reservar_inodo(in));
				
		const void *buf;
		buf = malloc(sizeof(64));
		printf("bytes escritos = %d\n\n", mi_write_f(0, buf, 0, 64));
		
		
		void *bufl;
		bufl = malloc(sizeof(64));
		printf("bytes leídos = %d\n", mi_read_f(0, bufl, 256, 64));*/
		
		
		
		/* PRUEBAS CON mi_truncar_f 
		printf("\n\n\n\n##################################################\n");
		printf("PRUEBAS CON mi_truncar_f\n\n");
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 20; 
		int pp;
		for (pp = 0; pp < TAM_PIND; pp++) {
			in.pb_ind[pp] = 0;
		}*/
		
		/* Punteros directos 
		unsigned int bufferp[TP];
		int j;
		for (j = 0; j < TAM_PDIR; j++) {
			in.pb_dir[j] = reservar_bloque();
		}
		printf("Punteros directos listos\n");
		leerSB();*/
		
		/* Puntero indirecto 0 
		in.pb_ind[0] = reservar_bloque();
		for (j = 0; j < TP; j++) {
			bufferp[j] = reservar_bloque();
		}
		bwrite(in.pb_ind[0], bufferp);
		printf("Puntero indirecto 0 listo\n");
		leerSB();*/
		
		/* Puntero indirecto 1 
		unsigned int bufferpp[TP];
		in.pb_ind[1] = reservar_bloque();
		for (j = 0; j < TP; j++) {
							
			int k;
			for (k = 0; k < TP; k++) {
				bufferpp[k] = reservar_bloque();
			}
			bufferp[j] = reservar_bloque();
			bwrite(bufferp[j], bufferpp);
		}
		bwrite(in.pb_ind[1], bufferp);
		printf("Puntero indirecto 1 listo\n");*/
		
		/* Hacemos las pruebas 
		leerSB();
		int perra = reservar_inodo(in);
		printf("PERRA: %d\n\n", perra);
		mi_truncar_f(0, 270336);*/
		
		/* Leemos el inodo 
		leer_inodo(&in, 0);
		int k;
		for (k = 0; k < 8 ; k++) {
			printf("dir: %d\n", in.pb_dir[k]);
		}
		for (k = 0; k < TAM_PIND; k++) {
			printf("ind: %d\n", in.pb_ind[k]);
		}*/
		
		
		
		/* PRUEBAS CON: extraer_camino 
		printf("\n\n\n\n##################################################\n");
		printf("PRUEBAS CON extraer_camino\n\n");
		
		char *prueba = "/hola";
		printf("prueba: %s\n", prueba);
		printf("tamaño de prueba = %d\n", strlen(prueba));
		printf("carácter [0] de prueba: %c\n", prueba[0]);
		printf("carácter [1] de prueba: %c\n", prueba[1]);
		
		//const char *camino = "/dir1/dir2/fichero";
		//const char *camino = "/dir/";
		const char *camino = "/fichero";
		char *inicial;
		char *final;
		int cam = extraer_camino(camino, inicial, final);
		if (cam == DIRECTORIO) {
			printf("DIRECTORIO, extraer_camino = %d\n", cam);
		}
		else if (cam == FICHERO) {
			printf("FICHERO, extraer_camino = %d\n", cam);
		}
		printf("inicial: %s\n", inicial);
		printf("final: %s\n", final);*/
		
		
		
		/* PRUEBAS CON: buscar_entrada 
		printf("\n\n\n\n##################################################\n");
		printf("PRUEBAS CON buscar_entrada\n\n");
		
		/* Inodo de "dir2" 
		struct inodo in;
		in.tipo = DIRECTORIO;
		in.n_bloques = 1;
		
		/* Entrada de "dir1"
		struct entrada ent;
		ent.inodo = reservar_inodo(in);
		strcpy(ent.nombre, "/dir1");
		/* Escribimos la entrada "dir1" en el inodo '/'
		mi_write_f(0, &ent, 0, sizeof(struct entrada));
		/* Comprobamos lo que hemos escrito 
		struct entrada e;
		mi_read_f(0, &e, 0, sizeof(struct entrada));
		printf("COMPROBACIÓN ESCRITURA DE dir1\n");
		printf(" - Directorio dir1: nombre = %s, inodo = %d\n\n", e.nombre, e.inodo);
		
		/* Entrada de "dir2" 
		//in.tipo = LIBRE;  NECESARIO SI COMENTAMOS LA ENTRADA DE FICHERO 
		ent.inodo = reservar_inodo(in);
		strcpy(ent.nombre, "/dir1/dir2/");
		/* Escribimos la entrada "dir2" en el inodo 1 
		mi_write_f(e.inodo, &ent, 0, sizeof(struct entrada));
		/* Comprobamos lo que hemos escrito 
		mi_read_f(0, &e, 0, sizeof(struct entrada));
		printf("COMPROBACIÓN DE ESCRITURA DE dir2\n");
		printf(" - Directorio dir1: nombre = %s, inodo = %d\n", e.nombre, e.inodo);
		mi_read_f(e.inodo, &e, 0, sizeof(struct entrada));
		printf(" - Directorio dir2: nombre = %s, inodo = %d\n\n", e.nombre, e.inodo); 
		
		/* Entrada de "fichero" 
		in.tipo = FICHERO;
		ent.inodo = reservar_inodo(in);
		strcpy(ent.nombre, "/dir1/dir2/fichero");
		/* Escribimos la entrada de "fichero" en el inodo 2 
		mi_write_f(e.inodo, &ent, 0, sizeof(struct entrada));
		/* Comprobamos lo que hemos escrito 
		mi_read_f(0, &e, 0, sizeof(struct entrada));
		printf("COMPROBACIÓN DE ESCRITURA DE dir2\n");
		printf(" - Directorio dir1 (%d): nombre = %s, inodo = %d\n", 0, e.nombre, e.inodo);
		mi_read_f(e.inodo, &e, 0, sizeof(struct entrada));
		printf(" - Directorio dir2 (%d): nombre = %s, inodo = %d\n", e.inodo, e.nombre, e.inodo);
		mi_read_f(e.inodo, &e, 0, sizeof(struct inodo));
		printf(" - Fichero fichero (%d): nombre = %s, inodo = %d\n", e.inodo, e.nombre, e.inodo);
		mi_read_f(1, &e, 0, sizeof(struct entrada));
		printf(" - Directorio dir2 (1): nombre = %s, inodo = %d\n", e.nombre, e.inodo);
		mi_read_f(0, &e, 0, sizeof(struct entrada));
		printf(" - Directorio dir1 (0): nombre = %s, inodo = %d\n\n", e.nombre, e.inodo); 
		
		/* Iniciamos las pruebas con 'buscar_entrada' 
		printf("VAMOS A BUSCAR ENTRADA\n");
		printf(" buscar_entrada()\n");
		unsigned int p_inodo;
		unsigned int p_entrada;
		unsigned int p_inodo_dir = 0;
		int puta = buscar_entrada("/dir1/dir2/fichero", &p_inodo_dir, &p_inodo, &p_entrada);
		printf(" Fin buscar_entrada()\n");
		/* Imprimimos los resultados obtenidos 
		printf("CONSULTAMOS LOS RESULTADOS OBTENIDOS\n");
		printf(" - p_inodo_dir: %d\n", p_inodo_dir);
		printf(" - p_inodo: %d\n", p_inodo);
		printf(" - p_entrada: %d\n", p_entrada); */
		
		
		
		/* PRUEBAS CON: extraer_directorio 
		char camino[60];
		strcpy(camino, "entramos mierda");
		char *inicial;
		inicial = (char *)malloc(sizeof(char *));
		extraer_directorio(camino, inicial);
		printf("\nCAMINO: camino = %s y inicial = %s\n", camino, inicial);*/
		
		
		/* PRUEBAS CON: mi_creat */
		
		
		
		/* FIN PRUEBAS */
		printf("\n\n\n");
		leerSB();
		bumount();
	}
	
	return (0);
}
