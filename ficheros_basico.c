#include "ficheros_basico.h"

int leerSB() {

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



int tamMB(unsigned int nbloques) {

	int tam_mb = nbloques;
	if ((tam_mb % (TB*8)) > 0) {
		tam_mb = tam_mb/(TB*8);
		tam_mb++;
	}
	else {
		tam_mb = tam_mb/(TB*8);
		if (nbloques == 0) {
			tam_mb++;
		}
	}
	
	return (tam_mb);
}



int tamAI(unsigned int nbloques) {

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



int initSB(unsigned int nbloques, char *nom_fs) {

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
	sb.n_bloques = nbloques;
	
	if (bwrite(0, &sb) < 0) {
		printf("ERROR AL INICIALIZAR EL SB (ficheros_basico.c -> initSB(%d, %s)): Error en bwrite(%d, &sb).\n", nbloques, nom_fs, 0);
		return (-1);
	}
	
	return (1);
}



int initMB(unsigned int nbloques) {

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



int initAI(unsigned int nbloques) {

	int n_inodos = nbloques/DIV_INODOS;
	int b_inodos = tamMB(nbloques) + 1;
	
	unsigned char buffer[TB];
	
	struct inodo in;
	in.tipo = LIBRE;
	in.t_bytes = 0;
	in.n_bloques = 0;
	
	int i;
	for (i = 0; i < n_inodos; i++) {
		in.f_creacion = time(NULL);
		in.f_modificacion = time(NULL);
		
		if (i == (n_inodos - 1)) {
			in.pb_ind[0] = 0;
		}
		else {
			in.pb_ind[0] = i+1;
		}
		
		memcpy(&buffer[(i%16)*64], &in, sizeof(struct inodo));
		
		if ((i%16 == 15) || (i == n_inodos - 1)) {
			bwrite(b_inodos, buffer);
			b_inodos++;
		}
	}
	
	/* Creamos el inodo raíz */
	in.tipo = DIRECTORIO;
	in.t_bytes = 0;
	in.n_bloques = 0;
	in.f_creacion = time(NULL);
	in.f_modificacion = time(NULL);
	reservar_inodo(in);
}



int pbit(unsigned int nbloque) {

	int i = nbloque % 8;
	return (i);
}



int pbyte(unsigned int nbloque) {

	int i = nbloque / 8;
	return (i);
}



int escribir_bit(unsigned int nbit, char valor) {

	int num_bloq_mb = tamMB(nbit); /* Sirve para traducir el bloque */
	
	unsigned char buffer[TB];
	if (bread(num_bloq_mb, buffer) < 0) {
		printf("ERROR AL ESCRIBIR BIT (ficheros_basico.c -> escribir_bit(%d, %d)): Error de lectura en bread(%d, buffer)\n", nbit, valor, num_bloq_mb);
		return (-1);
	}
	else if (DEBUG == 1) {
		printf("DEBUG | (escribir_bit): Lectura del bloque %d completada correctamente\n", num_bloq_mb);
	}
	
	int pos_bit = pbit(nbit);
	int pos_byte = pbyte(nbit);
	
	unsigned char mascara = 128;
	if (pos_bit > 0) {
		mascara >>= pos_bit;
	}
	
	if (valor == 1) {
		buffer[pos_byte%TB] |= mascara;
	}
	else if (valor == 0) {
		buffer[pos_byte%TB] &= ~mascara;
	}
	
	if (bwrite(num_bloq_mb, buffer) < 0) {
		printf("ERROR AL ESCRIBIR BIT (escribir_bit): Bloque %d, valor %d\n", num_bloq_mb, valor);
		return (-1);
	}
	else if (DEBUG == 1) {
		printf("DEBUG | (ficheros_basico.c -> escribir_bit(%d, %d)): Bit escrito correctamente.\n", nbit, valor);
	}
	
	return (1);
}



int leer_bit(unsigned int nbit) {

	int n_bloque = tamMB(nbit);
	
	unsigned char buffer[TB];
	if (bread(n_bloque, buffer) < 0) {
		printf("ERROR AL LEER BIT (ficheros_basico.c -> leer_bit(%d)): Error en bread(%d, buffer)\n", nbit, n_bloque);
		return (-1);
	}
	
	int pos_bit = pbit(nbit);
	int pos_byte = pbyte(nbit);
	
	unsigned char mascara = 128;
	
	if (buffer[pos_byte%TB] == 0) {
		return (0);
	}
	else if (buffer[pos_byte%TB] == 255) {
		return (1);
	}
	else {
		if (pos_bit > 0) {
			mascara >>= pos_bit;
		}
		if (buffer[pos_byte%TB] & mascara) {
			return (1);
		}
		else {
			return (0);
		}
	}
}



int reservar_bloque() {

	struct superbloque SB;
	bread(0, (char *)&SB);
	
	if (SB.b_libres > 0) {
		int bdatos = SB.primerb_dt;
		int bloque = tamMB(bdatos);
		int pos_byte = pbyte(bloque);
		
		unsigned char buffer[TB];
		bread(bloque, buffer);
		
		while (buffer[pos_byte%TB] == 255) {
			pos_byte++;
			
			if ((pos_byte%TB) == 0) {
				bloque++;
				bread(bloque, buffer);
				pos_byte++;
			}
		}
		
		unsigned char mascara = 128;
		int i = 0;
		while (buffer[pos_byte%TB] & mascara){
			buffer[pos_byte%TB] <<= 1;
			i++;
		}
		
		pos_byte *= 8;
		pos_byte += i;
		
		if (escribir_bit(pos_byte, 1) < 0) {
			printf("ERROR AL RESERVAR BLOQUE (reservar_bloque()): Error al escribir_bit(%d, 1)\n", pos_byte);
			return (-1);
		}
		
		SB.b_libres--;
		if (bwrite(0, (char *)&SB) < 0) {
			printf("ERROR AL ACTUALIZAR EL SUPERBLOQUE (ficheros_basico.c -> reservar_bloque()): Error al bwrite(0, SB)\n");
			return (-1);
		}
		
		return (pos_byte);
	}
	else {
		printf("ERROR, NO HAY BLOQUES LIBRES\n");
		return (-1);
	}
}



int liberar_bloque(unsigned int bloque) {

	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_bloque(%d)): Error al leer el SB\n", bloque);
		return (-1);
	}

	if ((0 <= bloque) && (bloque < SB.n_bloques)) {
		if (leer_bit(bloque) == 1) {
			if (escribir_bit(bloque, 0) < 0) {
				printf("ERROR AL LIBERAR BLOQUE (ficheros_basico.c -> liberar_bloque(%d)): Error al escribir_bit(%d, 0)\n", bloque, bloque);
				return (-1);
			}
			
			struct superbloque SB;
			if (bread(0, (char *)&SB) < 0) {
				printf("ERROR AL GUARDAR SUPERBLOQUE (ficheros_basico.c -> liberar_bloque(&d)): Error al bread(0, SB)\n", bloque);
				return (-1);
			}
			
			if ((SB.ultimob_dt - SB.ultimob_ai) > SB.b_libres) {
				SB.b_libres++;
				bwrite(0, (char *)&SB);
			}
		}
		else {
			printf("Info (ficheros_basico.c -> liberar_bloque): El bloque %d que quieres liberar está libre.\n", bloque);
		}
		return (1);
	}
	else {
		printf("ERROR (ficheros_basico.c -> liberar_bloque(%d)): Valor de bloque negativo ó fuera de rango.\n", bloque);
		return (-1); /* PROVISIONAL, ANALIZAR SI ES ÉXITO Ó FRACASO QUE EL BLOQUE DESTINO NO EXISTA */
	}
}



int escribir_inodo(struct inodo in, unsigned int ninodo) {

	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR AL LEER SB (ficheros_basico.c -> escribir_inodo(in, %d)): Error al bread(0, SB)\n", ninodo);
		return (-1);
	}
	
	if (SB.i_libres > 0) {
		if ((ninodo < 0) || (ninodo <= SB.n_inodos)) {
			unsigned int b_inodo = ninodo/16;
			b_inodo += SB.primerb_ai;
			
			unsigned char buffer[TB];
			if (bread(b_inodo, buffer) < 0) {
				printf("ERROR (ficheros_basico.c -> escribir_inodo(in, %d)): Error en bread(%d, buffer)\n", ninodo, b_inodo);
				return (-1);
			}
			
			int byte_inodo = (ninodo*64)%TB;
			if (memcpy(&buffer[byte_inodo], &in, sizeof(struct inodo)) < 0) {
				printf("ERROR (ficheros_basico.c -> escribir_inodo(in, %d)): Error en memcpy(buffer[%d], in, 64)\n", ninodo, byte_inodo);
				return (-1);
			}
			
			if (bwrite(b_inodo, buffer) < 0) {
				printf("ERROR (ficheros_basico.c -> escribir_inodo(in, %d)): Error en bwrite(%d, buffer)\n", ninodo, b_inodo);
				return (-1);
			}
			else if (DEBUG == 1) {
				printf("DEBUG (ficheros_basico.c -> escribir_inodo(in, %d)): Inodo escrito con éxito!\n", ninodo);
			}
			
			return (1);
		}
		else {
			printf("Info (ficheros_basico.c -> escribir_inodo(in, %d)): Upps! parece que el inodo %d no cabe en nuestro array de %d inodos.\n", ninodo, ninodo, SB.n_inodos);
			return (-1); /* DUDA SI ESTO ES NECESARIO */
		}
	}
	else {
		printf("Info (ficheros_basico.c -> escribir_inodo(in, %d)): Upps! parece que no hay más inodos libres.\n");
		return (-1); /* DUDA SI ESTO ES NECESARIO */
	}

}



int leer_inodo(struct inodo *in, unsigned int ninodo) {

	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR AL LEER EL SB (ficheros_basico.c -> leer_inodo(in, %d)): Error al leer el superbloque\n", ninodo);
		return (-1);
	}
	
	if ((ninodo <= SB.n_inodos) && (ninodo >= 0)) {
		unsigned char buffer[TB];
		unsigned int b_inodo = ninodo/16;
		b_inodo += SB.primerb_ai;
		
		if (bread(b_inodo, buffer) < 0) {
			printf("ERROR (ficheros_basico.c -> leer_inodo(in, %d)): Error al leer el bloque %d\n", ninodo, b_inodo);
			return (-1);
		}
		
		unsigned int byte_inodo = (ninodo*64)%TB;
		if (memcpy(in, &buffer[byte_inodo], sizeof(struct inodo)) < 0) {
			printf("ERROR (ficheros_basico.c -> leer_inodo(in, %d)): Error al hacer memcpy(byte_inodo: %d)\n", ninodo, byte_inodo);
			return (-1);
		}
		else if (DEBUG == 1) {
			printf("DEBUG (ficheros_basico.c -> leer_inodo(in, %d)): Inodo leído con éxito\n", ninodo);
		}
		
		return (1);
	}
	else {
		printf("Info (ficheros_basico.c -> leer_inodo(in, %d)): Upps! parece que el numero de inodo es muy grande o menor que 0.\n", ninodo);
		return (-1); /* DUDA SI ESTO ES NECESARIO */
	}
}



int reservar_inodo(struct inodo in) {

	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR (ficheros_basico.c -> reservar_inodo(in)): Error al leer el superbloque\n");
		return (-1);
	}
	
	if (SB.i_libres > 0) {
		int inodo_libre = SB.ni_libre;
		struct inodo in_libre;
		
		if (leer_inodo(&in_libre, inodo_libre) < 0) {
			printf("ERROR (ficheros_basico.c -> reservar_inodo(in)): Error al leer el primer inodo libre\n");
			return (-1);
		}
		
		/* Inicializamos los punteros del nuevo inodo */
		int i;
		for (i = 0; i < TAM_PDIR; i++) {
			if ((0 <= i) && (i < TAM_PIND)) {
				in.pb_ind[i] = 0;
			}
			
			in.pb_dir[i] = 0;
		}		
		
		if (escribir_inodo(in, inodo_libre) < 0) {
			printf("ERROR (ficheros_basico.c -> reservar_inodo(in)): Error al escribir el inodo\n");
			return (-1);
		}
		
		SB.ni_libre = in_libre.pb_ind[0];
		SB.i_libres--;
		if (bwrite(0, (char *)&SB) < 0) {
			printf("ERROR (ficheros_basico.c -> reservar_inodo(in)): Error al guardar el superbloque\n");
			return (-1);
		}
		
		return (inodo_libre);
	}
	else {
		printf("Info (ficheros_basico.c -> reservar_inodo(in)): No quedan inodos libres\n");
		return (-1); /* DUDA SI ESTO ESTA BIEN AQUI */
	}

}



int liberar_bloques(unsigned int ninodo, unsigned int nbytes) {

	struct inodo in;
	if (leer_inodo(&in, ninodo) < 0) {
		printf("ERORR (ficheros_basico.c -> liberar_bloques(%d, %d)): Error al leer el inodo %d\n", ninodo, nbytes, ninodo);
		return (-1);
	}
	
	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_bloques(%d, %d)): Error al leer el SB\n", ninodo, nbytes);
		return (-1);
	}
	
	if (in.tipo != LIBRE) {
		int bloques_liberados = 0;
		int n_asignados = in.n_bloques;
		int pos_inicial;
		
		if (nbytes%TB > 0) {
			pos_inicial = (nbytes/TB)+1;
		}
		else {
			pos_inicial = nbytes/TB;
		}
		
		if (n_asignados > 0) {
		
			/* Eliminamos los bloques directos */
			int i;
			if ((0 <= pos_inicial) && (pos_inicial < TAM_PDIR)) {
				for (i = pos_inicial; i < TAM_PDIR; i++) {
				
					if (in.pb_dir[i] > 0) {
						if (liberar_bloque(in.pb_dir[i]) < 0) {
							printf("ERROR (ficheros_basico.c -> liberar_bloques(%d, %d)): Error al liberar el bloque %d\n", ninodo, nbytes, in.pb_dir[i]);
							return (-1);
						}
						else {
							in.pb_dir[i] = 0;
							bloques_liberados++;
							in.n_bloques--;
						}
					}
					
				}
				pos_inicial = i; /* Avanzamos la posición inicial */
			}
			
			/* Eliminamos los bloques indirectos */
			int n_max;
			int j;
			for (j = 0; j < TAM_PIND; j++) {
				if ((0 < in.pb_ind[j]) && (in.pb_ind[j] < SB.n_bloques) && (leer_bit(in.pb_ind[j]) > 0)) {
				
					n_max = j+1;
					int liberados = liberar_bloques_indirectos(in.pb_ind[j], 1, n_max, pos_inicial);
					
					if (liberados == TP) {
						if (liberar_bloque(in.pb_ind[j]) < 0) {
							printf("ERROR (ficheros_basico.c -> liberar_bloques(%d, %d)): Error al liberar el bloque %d del puntero indirecto %d\n", ninodo, nbytes, in.pb_ind[j], j);
							return (-1);
						}
						printf("liberamos el puntero indirecto j = %d\n", j);
						in.pb_ind[j] = 0;
						/* FALTA ACTUALIZAR EL INODO, BLOQUES LIBERADOS, ETC... */
					}
				}
				else { /* QUIZÁS ESTO SOBRE TAMBIÉN */
					printf("Info (ficheros_basico.c -> liberar_bloques(%d, %d)): No hay puntero indirecto (fuera de rango) %d\n", ninodo, nbytes, j);
				}
			}			
			
			/* Actualizamos el inodo */
			int n_bloques;
			if (nbytes%TB > 0) {
				n_bloques = (nbytes/TB) + 1;
			}
			else {
				n_bloques = nbytes/TB;
			}
			
			in.n_bloques = n_bloques;
			in.t_bytes = nbytes;
			in.f_modificacion = time(NULL);
			
			escribir_inodo(in, ninodo);
			
		}
		else {
			printf("Info (ficheros_basico.c -> liberar_bloques(%d, %d)): El inodo %d no tiene bloques asignados\n", ninodo, nbytes, ninodo);
			return (-1); /* DUDA SI ESTO HAY QUE PONERLO */
		}
		
	}
	else {
		printf("Info (ficheros_basico.c -> liberar_bloques(%d, %d)): El inodo %d está libre\n", ninodo, nbytes, ninodo);
		return (-1); /* DUDA SI ESTO HAY QUE PONERLO */
	}

	return (1);
}



int liberar_bloques_indirectos(unsigned int pos_inicial, unsigned int nivel, unsigned int n_max, int nbloque) {

	if (nivel <= n_max) {
	
		unsigned int bufferp[TP];
		if (bread(pos_inicial, bufferp) < 0) {
			printf("ERROR (ficheros_basico.c -> liberar_bloques_indirectos(%d, %d, %d, %d)): Error al leer el bloque %d en el caso general\n", pos_inicial, nivel, n_max, nbloque, nbloque);
			return (-1);
		}
		
		switch (nivel) {
			case 1:
				if (n_max == 1) {
					nbloque -= TAM_PDIR;
					pos_inicial = nbloque;
				}
				else if (n_max == 2) {
					nbloque -= (TP+TAM_PDIR);
					pos_inicial = nbloque/TP;
				}
				else if (n_max == 3) {
					nbloque -= ((TP*TP)+TP+TAM_PDIR);
					pos_inicial = nbloque/(TP*TP);
				}
				
				break;
				
				
			case 2:
				if (n_max == 2) {
					pos_inicial = nbloque%TP;
				}
				else if (n_max == 3) {
					nbloque = nbloque%(TP*TP);
					pos_inicial = nbloque/TP;
				}	
				
				break;
				
			
			case 3:
				pos_inicial = nbloque%TP;
				
				break;
				
				
			default:
				printf("Info (ficheros_basico.c -> liberar_bloques_indirectos(%d, %d, %d, %d)): Switch en default \n", pos_inicial, nivel, n_max, nbloque);
				break;
		}
		
		
		
		/* Si el cálculo es negativo, inicializamos */
		if (nbloque < 0) {
			nbloque = 0;
			pos_inicial = 0;
		}

		/* Recorremos el resto del bloque */
		int b_liberados = 0;
		int i;
		for (i = pos_inicial; i < TP; i++) { /* NBLOQUE POR POS_INICIAL */
		
			nivel++;
			int v_devuelto = liberar_bloques_indirectos(bufferp[i], nivel, n_max, nbloque);
			nivel--;
			
			/* Si hemos liberado 256 bloques, liberamos el bloque de punteros */
			if ((n_max == nivel) || (v_devuelto == TP)) {
				if ((bufferp[i] == 0) || (liberar_bloque(bufferp[i]) < 0)) {
					printf("ERROR (ficheros_basico.c -> liberar_bloques_indirectos(%d, %d, %d, %d)): Error al liberar el bloque %d en el caso general\n", pos_inicial, nivel, n_max, nbloque, pos_inicial);
					return (-1);
				}
				b_liberados++;
			}
			nbloque = 0;
		}
		
		return (b_liberados);
	}
	else {
		return (0);
	}
}



int liberar_inodo(unsigned int ninodo) {

	if (liberar_bloques(ninodo, 0) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_inodo(%d)): Error al liberar el inodo %d\n", ninodo, ninodo);
		return (-1);
	}
	
	/* Actualizamos la lista enlazada de inodos libres */
	struct superbloque SB;
	if (bread(0, (char *)&SB) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_inodo(%d)): Error al leer el superbloque\n", ninodo);
		return (-1);
	}
	
	struct inodo in;
	if (leer_inodo(&in, ninodo) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_inodo(%d)): Error al leer el inodo\n", ninodo);
		return (-1);
	}
	
	in.pb_ind[0] = SB.ni_libre;
	in.tipo = LIBRE;
	in.t_bytes = 0;
	in.n_bloques = 0;
	in.f_modificacion = time(NULL);
	
	if (escribir_inodo(in, ninodo) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_inodo(%d)): Error al escribir el inodo %d\n", ninodo, ninodo);
		return (-1);
	}
	
	SB.ni_libre = ninodo;
	SB.i_libres++;
	
	if (bwrite(0, (char *)&SB) < 0) {
		printf("ERROR (ficheros_basico.c -> liberar_inodo(%d)): Error al escribir el superbloque\n", ninodo);
		return (-1);
	}
	
	return (1);
}



int traducir_bloque_inodo(unsigned int ninodo, unsigned int blogico, char reservar) {

	if ((0 <= blogico) && (blogico <= (TAM_PDIR+TP+(TP*TP)+(TP*TP*TP)))) {
		if ((reservar == 1) || (reservar == 0)) {
		
			struct inodo in;
			if (leer_inodo(&in, ninodo) < 0) {
				printf("ERROR (ficheros_basico.c -> traducir_bloque_inodo(%d, %d, %d)): Error al leer el inodo %d\n", ninodo, blogico, reservar, ninodo);
				return (-1);
			}
			
			/* Punteros directos */
			if ((0 <= blogico) && (blogico < TAM_PDIR)) {
				if (reservar == 0) {
					if (in.pb_dir[blogico] <= 0) {
						return (-1);
					}
					else {
						return (in.pb_dir[blogico]);
					}
				}
				else if (reservar == 1) {
					if (in.pb_dir[blogico] <= 0) {
						int bloque_nuevo = reservar_bloque();
						in.pb_dir[blogico] = bloque_nuevo;
						escribir_inodo(in, ninodo);
						
						return (bloque_nuevo);
					}
					else {
						return (in.pb_dir[blogico]);
					}
				}
			}
			
			/* Punteros indirectos */
			else if ((TAM_PDIR <= blogico) && (blogico <= (TAM_PDIR+TP+(TP*TP)+(TP*TP*TP)))) {
			
				//printf("Punteros indirectos\n");
				
				int puntInd = 0;
				if ((TAM_PDIR <= blogico) && (blogico < (TAM_PDIR+TP))) {
					puntInd = 0;
					//printf("    Puntero 0\n");
				}
				else if (((TAM_PDIR+TP) <= blogico) && (blogico < (TAM_PDIR+TP+(TP*TP)))) {
					puntInd = 1;
					//printf("    Puntero 1\n");
				}
				else if (((TAM_PDIR+TP+(TP*TP)) <= blogico) && (blogico < (TAM_PDIR+TP+(TP*TP)+(TP*TP*TP)))) {
					puntInd = 2;
					//printf("    Puntero 2\n");
				}
				
				/* Si el bloque de punteros indirectos no existe */
				if (in.pb_ind[puntInd] <= 0) {
					if (reservar == 0) {
						//printf("	bloque de punteros inexistente (reservar 0, devolvemos 0)\n");
						return (0);
					}
					else if (reservar == 1) {
						//printf("	bloque de punteros inexistente (reservar 1, 1 bloque menos)\n");
					
						unsigned int bufferp[TP];
						int i;
						for (i = 0; i < TP; i++) {
							bufferp[i] = 0;
						}
						
						in.pb_ind[puntInd] = reservar_bloque();
						//printf("	escribimos el bloque de punteros inexistente %d\n", in.pb_ind[puntInd]);
						if (bwrite(in.pb_ind[puntInd], bufferp) < 0) {
							printf("ERROR (ficheros_basico.c -> traducir_bloque_inodo(%d, %d, %c)): Error al escribir el bloque de punteros nuevo en %d, con nbloque %d\n", ninodo, blogico, reservar, puntInd, in.pb_ind[puntInd]);
							return (-1);
						}
						
						/* Actualizamos el inodo */
						escribir_inodo(in, ninodo);
					}
				}
				return (traducir_puntero_indirecto(blogico, 1, ++puntInd, reservar, 0, in.pb_ind[puntInd]));
			}
		}
	}

}



int traducir_puntero_indirecto(unsigned int blogico, unsigned int nivel, unsigned int n_max, char reservar, int pos_inicial, int pos_anterior) {

	if (nivel <= n_max) {
	
		int v_devuelto;
		
		unsigned int bufferp[TP];
		if (bread(pos_anterior, bufferp) < 0) {
			printf("ERROR (ficheros_basico.c -> traducir_puntero_indirecto(%d, %d, %d, %s, %d, %d)): Error al leer el bloque pos_anterior: %d\n", blogico, nivel, n_max, reservar, pos_inicial, pos_anterior, pos_anterior);
			return (-1);
		}
		
		/* HACEMOS EL SWITCH CON NIVEL */
		switch (nivel) {
			case 1:
				if (n_max == 1) {
					blogico -= TAM_PDIR;
					pos_inicial = blogico;
				}
				else if (n_max == 2) {
					blogico -= (TP+TAM_PDIR);
					pos_inicial = blogico/TP;
				}
				else if (n_max == 3) {
					blogico -= ((TP*TP)+TP+TAM_PDIR);
					pos_inicial = blogico/(TP*TP);
				}
				
				break;
				
				
			case 2:
				if (n_max == 2) {
					pos_inicial = blogico%TP;
				}
				else if (n_max == 3) {
					blogico = blogico%(TP*TP);
					pos_inicial = blogico/TP;
				}
				
				break;
				
			
			case 3:
				pos_inicial = blogico%TP;
				
				break;
		}
		
		/* Si el cálculo es negativo, inicializamos 
		if (blogico < 0) {
			blogico = 0;
			pos_inicial = 0;
		}*/
		
		/* NO EXISTE EL BLOQUE CALCULADO */
		if (bufferp[pos_inicial] <= 0) {
			if (reservar == 0) {
				return (-1);
			}
			else if (reservar == 1) {
				bufferp[pos_inicial] = reservar_bloque();
				if (bwrite(pos_anterior, bufferp) < 0) {
					printf("ERROR (ficheros_basico.c -> traducir_puntero_indirecto(%d, %d, %d, %s, %d, %d)): Error al escribir en el bloque pos_anterior: %d\n", blogico, nivel, n_max, reservar, pos_inicial, pos_anterior, pos_anterior);
					return (-1);
				}
				
				if (n_max != nivel) {
					unsigned int bufferpp[TP];
					int i;
					for (i = 0; i < TP; i++) {
						bufferpp[i] = 0;
					}
					
					if (bwrite(bufferp[pos_inicial], bufferpp) < 0) {
						printf("ERROR (ficheros_basico.c -> traducir_puntero_indirecto(%d, %d, %d, %s, %d, %d)): Error al escribir el nuevo bloque en bufferp[%d] = %d\n", blogico, nivel, n_max, reservar, pos_inicial, pos_anterior, pos_inicial, bufferp[pos_inicial]);
						return (-1);
					}
				}
			}
		}
			
		if (n_max != nivel) {
			return (traducir_puntero_indirecto(blogico, ++nivel, n_max, reservar, 0, bufferp[pos_inicial]));
		}
		else {
			return (bufferp[pos_inicial]);
		}
			
	}
	else {
		return (0);
	}
}



