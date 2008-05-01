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
		if (nbloques == 0) {
			tam_mb++;
		}
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



int initAI(unsigned int nbloques)
{

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
	
	/*in.tipo = DIRECTORIO;
	in.t_bytes = 0;
	in.n_bloques = 0;
	in.f_creacion = time(NULL);
	in.f_modificacion = time(NULL);
	//reservar_inodo(in);	*/
	
}



int pbit(unsigned int nbloque) 
{

	int i = nbloque % 8;
	return (i);

}



int pbyte(unsigned int nbloque)
{

	int i = nbloque / 8;
	return (i);

}



int escribir_bit(unsigned int nbit, char valor)
{

	int num_bloq_mb = tamMB(nbit); /* Sirve para traducir el bloque */
	
	unsigned char buffer[TB];
	if (bread(num_bloq_mb, buffer) < 0) {
		printf("ERROR AL ESCRIBIR BIT (ficheros_basico.c -> escribir_bit(%d, %d)): Error de lectura en bread(%d, buffer)\n", nbit, valor, num_bloq_mb);
		return (-1);
	}
	else if (DEBUG == 1) {
		printf("DEBUG | (escribir_bit): Lectura del bloque %d completada correctamente", num_bloq_mb);
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



int leer_bit(unsigned int nbit)
{

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



int reservar_bloque()
{

	struct superbloque SB;
	bread(0, (char *)&SB);
	
	if (SB.b_libres > 0) {
		int bdatos = SB.primerb_dt;
		int bloque = tamMB(bdatos);
		int pos_byte = pbyte(bloque);
		
		unsigned char buffer[TB];
		bread(bloque, buffer);
		
		while (buffer[pos_byte%TB] == 255){
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



int liberar_bloque(unsigned int bloque)
{

	if (bloque >= 0) {
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
			
			if ((SB.n_bloques - SB.ultimob_ai) > SB.b_libres) {
				SB.b_libres++;
				bwrite(0, (char *)&SB);
			}
		}
		else {
			printf("Info (liberar_bloque): El bloque %d que quieres liberar está libre.\n", bloque);
		}
		return (1);
	}
	else {
		printf("ERROR (liberar_bloque(%d)): Valor de bloque negativo.\n", bloque);
		return (-1);
	}

}



int escribir_inodo(struct inodo in, unsigned int ninodo)
{

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



int leer_inodo(struct inodo *in, unsigned int ninodo)
{

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



int reservar_inodo(struct inodo in)
{

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



int liberar_bloques(unsigned int ninodo, unsigned int nbytes)
{

	struct inodo in;
	if (leer_inodo(&in, ninodo) < 0) {
		printf("ERORR (ficheros_basico.c -> liberar_bloques(%d, %d)): Error al leer el inodo %d\n", ninodo, nbytes, ninodo);
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
			int j;
			for (j = 0; j < TAM_PIND; j++) {
				/* LIBERAMOS BLOQUES INDIRECTOS */
			}
			
			
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

}





















