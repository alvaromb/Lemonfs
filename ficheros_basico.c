#include "ficheros_basico.h"


int leerSB()
{
	struct superbloque SB;
	
	bread(0, (char *)&SB);
	
	printf("/***********************************/\n");
	printf(" Datos del SuperBloque:\n\n");
	
	printf("	Nombre del disco: 	%s\n", SB.nom_fs);
	printf("	1er bloque del MB: 	%d\n", SB.primerb_mb);
	printf("	Ult. bloque del MB:	%d\n", SB.ultimob_mb);
	printf("	1er bloque del AI:	%d\n", SB.primerb_ai);
	printf("	Ult. bloque del AI:	%d\n", SB.ultimob_ai);
	printf("	1er bloque de datos:	%d\n", SB.primerb_dt);
	printf("	Ult. bloque de datos:	%d\n\n", SB.ultimob_dt);
	
	printf("	Inodo 'root': 	%d\n", SB.ni_root);
	printf("	Inodo libre: 	%d\n\n", SB.ni_libre);
	
	printf("	Num. bloques libres:	%d\n", SB.b_libres);
	printf("	Num. inodos libres:	%d\n\n", SB.i_libres);
	
	printf("	Total bloques:	%d\n", SB.n_bloques);
	printf("	Total inodos:	%d\n\n", SB.n_inodos);
	
	printf("/***********************************/\n");
	
	return (1);
}


int tamMB(unsigned int nbloques)
{
	int tam_mb = nbloques;
	if ((tam_mb % (TB*8)) > 0) {
		tam_mb = tam_mb/(TB*8);
		tam_mb++;
	}
	else {
		tam_mb = tam_mb/(TB*8);
	}
	
	return (tam_mb);
}


int tamAI(unsigned int nbloques)
{
	int tam_in = (nbloques/DIV_INODOS)*64;
	if ((tam_in % TB) == 0) {
		tam_in = tam_in/TB;
	}
	else {
		tam_in = tam_in/TB;
		tam_in++;
	}
	
	return (tam_in);
}


int initSB(unsigned int nbloques, char *nom_fs)
{
	struct superbloque sb;
	int tam_mb = tamMB(nbloques);
	int tam_ai = tamAI(nbloques);
	int n_inodos = nbloques/DIV_INODOS;
	
	strncpy(sb.nom_fs, nom_fs, sizeof(sb.nom_fs));
	sb.primerb_mb = 1;
	sb.ultimob_mb = tam_mb;
	sb.primerb_ai = tam_mb + 1;
	sb.ultimob_ai = tam_mb + tam_ai;
	sb.primerb_dt = tam_mb + tam_ai + 1;
	sb.ultimob_dt = nbloques - 1;
	sb.ni_root = 0;
	sb.ni_libre = 0;
	sb.b_libres = sb.ultimob_dt - sb.ultimob_ai;
	sb.i_libres = n_inodos;
	sb.n_inodos = n_inodos;
	
	if (bwrite(0, &sb) < 0) {
		printf("ERROR AL INICIALIZAR EL SB (ficheros_basico.c -> initSB(%d, %s)): Error en bwrite(%d, &sb).", nbloques, nom_fs, 0);
		return (-1);
	}
	
	return (1);
}


int initMB(unsigned int nbloques)
{
	unsigned char buffer[TB];
	
	int b_mb = tamMB(nbloques);
	int b_ai = tamAI(nbloques);
	int ultimob_ai = 1 + b_mb + b_ai;
	
	int b_byte = ultimob_ai/8;
	int b_bit = ultimob_ai%8;
	
	int bloq = 0;
	
	
	/* Bytes con 1's */
	int i;
	for (i = 0; i < b_byte; i++) {
		buffer[i%TB] = 255;
		if (((i%TB == 0)) && (i != 0)) {
			bloq++;
			bwrite(bloq, buffer);
		}
	}
	
	/* Byte con 0's y 1's */
	if (b_bit > 0) {
		int b_escribir = b_byte%TB;
		unsigned char valor;
		
		for (i = 0; i < 8; i++) {
			valor = 128;
			
			if (i < b_bit) {
				if (i > 0) {
					valor >>= i;
				}
				buffer[b_escribir] |= valor;
			}
			else {
				if (i > 0) {
					valor >>= i;
				}
				buffer[b_escribir] &= ~valor;
			}
		}
		b_byte++;
	}
	
	/* Bytes con 0's */
	i = b_byte;
	while (i < b_mb*TB) {
		buffer[i%TB] = 0;
		i++;
		if ((i%TB) == 0) {
			bloq++;
			bwrite(bloq, buffer);
		}
	}
}





















