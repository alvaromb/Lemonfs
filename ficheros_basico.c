#include "ficheros_basico.h"


int leerSB()
{
	struct superbloque SB;
	
	bread(0, (char *)&SB);
	
	printf("/***********************************/\n");
	printf(" Datos del SuperBloque:\n\n");
	
	printf("	Nombre del disco: 		%s\n", SB.nom_fs);
	printf("	1er bloque del MB: 		%d\n", SB.primerb_mb);
	printf("	Ult. bloque del MB:		%d\n", SB.ultimob_mb);
	printf("	1er bloque del AI:		%d\n", SB.primerb_ai);
	printf("	Ult. bloque del AI:		%d\n", SB.ultimob_ai);
	printf("	1er bloque de datos:	%d\n", SB.primerb_dt);
	printf("	Ult. bloque de datos:	%d\n\n", SB.ultimob_dt);
	
	printf("	Inodo 'root': 			%d\n", SB.ni_root);
	printf("	Inodo libre: 			%d\n\n", SB.ni_libre);
	
	printf("	Num. bloques libres:	%d\n", SB.b_libres);
	printf("	Num. inodos libres:		%d\n\n", SB.i_libres);
	
	printf("	Total bloques:		%d\n", SB.n_bloques);
	printf("	Total inodos:		%d\n\n", SB.n_inodos);
	
	printf("/***********************************/\n");	
}


int tamMB(unsigned int nbloques)
{
	int tam_mb = nbloques
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

























