#include "ficheros.h"

int mi_write_f(unsigned int inodo, const void *buf, unsigned int offset, unsigned int nbytes) {
	
	int primer_byte = offset;
	int ultimo_byte = offset + nbytes - 1;
	int primer_blogico = offset/TB;
	int ultimo_blogico = ultimo_byte/TB;
	int aux = 0;
	int byte_ini = primer_byte%TB;
	int byte_fin = ultimo_byte%TB;
	int bytes_escritos = 0;
	int primer_ultimo = 0;	/* PROVISIONAL!!! PARA VER ERRORES */
	unsigned char buffer[TB];
	
	/* Valores calculados 
	printf("/-------------------------------------/\n");
	printf("	primer_byte 	= %d\n", primer_byte);
	printf("	ultimo_byte 	= %d\n", ultimo_byte);
	printf("	primer_blogico 	= %d\n", primer_blogico);
	printf("	ultimo_blogico 	= %d\n", ultimo_blogico);
	printf("/-------------------------------------/\n"); */
	
	
	int i;
	for (i = primer_blogico; i <= ultimo_blogico; i++) {
	
		//printf("Vamos a traducir_bloque_inodo(%d, %d, 1)\n", inodo, i);
		aux = traducir_bloque_inodo(inodo, i, 1);
		//printf("TOCAMOS EL BLOQUE %d EN ESCRITURA\n", aux);
		
		if ((primer_blogico != ultimo_blogico) && (i == primer_blogico)) {
			//printf("- entra en primer_blogico con aux = %d\n", aux);
			bread(aux, buffer);
			//printf("	byte_ini: %d\n",byte_ini);
			memcpy(buffer + byte_ini, buf, TB - byte_ini); /* meter control de errores */
			//printf("	memcpy(%d + %d, %d, %d - %d)\n\n", buffer, byte_ini, buf, TB, byte_ini);
			primer_ultimo++;
		}
		else if ((primer_blogico == ultimo_blogico) || (i == ultimo_blogico)) {
			//printf("- entra en ultimo_blogico con aux = %d\n", aux);
			bread(aux, buffer);
			//printf("	byte_fin: %d\n",byte_fin);
			
			if (primer_blogico == ultimo_blogico) {
				memcpy(buffer+byte_ini, buf, byte_fin+1); /* meter control de errores */
				//printf("	SOLO 1 BLOQUE memcpy(%d+%d, %d, %d)\n\n", buffer, byte_ini, buf, byte_fin);
			}
			else {
				memcpy(buffer, buf+(TB-byte_ini)+(ultimo_blogico-primer_blogico-1)*TB, byte_fin+1); /* meter control de errores */
				//printf("	memcpy(%d, %d+(%d-%d)+%d, %d)\n\n", buffer, buf, TB, byte_ini, (ultimo_blogico-primer_blogico-1)*TB, byte_fin+1);
			}
			primer_ultimo++;
		}
		else {
			//printf("- entra en el else con aux = %d\n", aux);
			memcpy(buffer, buf+(TB-byte_ini)+(i-primer_blogico-1)*TB, TB); /* meter control de errores */
			//printf("	memcpy(%d, %d, %d)\n\n", buffer, buf+(TB-byte_ini)+(i-primer_blogico-1)*TB, TB);
			bytes_escritos++;
		}
		
		bwrite(aux, buffer);
	}
	
	//printf("(antes) bytes_escritos = %d (bloques del else) + %d (extremos)\n", bytes_escritos, primer_ultimo);	
	
	if (primer_blogico != ultimo_blogico) {
		bytes_escritos *=TB;
		bytes_escritos = bytes_escritos + (TB-byte_ini) + (byte_fin+1); /* en 1 o menos falla */
	}
	else {
		bytes_escritos = (++byte_fin) - byte_ini;
	}
	//printf("(despues) bytes_escritos = %d (bloques del else)\n\n\n\n", bytes_escritos);
	
	/* Actualizamos los datos del inodo */
	struct inodo in;
	if (leer_inodo(&in, inodo) < 0) {
		printf("ERROR (ficheros.c -> mi_write_f(%d, buf, %d, %d)): Error al leer el inodo %d para actualizar sus datos.\n", inodo, offset, nbytes, inodo);
		return (-1);
	}
	
	in.f_modificacion = time(NULL);
	in.t_bytes = bytes_escritos+offset;
	
	if (in.t_bytes%TB > 0) {
		in.n_bloques = (in.t_bytes%TB) + 1;
	}
	else {
		in.n_bloques = in.t_bytes%TB;
	}
	
	if (escribir_inodo(in, inodo) < 0) {
		printf("ERROR (ficheros.c -> mi_write_f(%d, buf, %d, %d)): Error al escribir el inodo %d para actualizar sus datos.\n", inodo, offset, nbytes, inodo);
		return (-1);
	}
	
	return (bytes_escritos);	
}


int mi_read_f(unsigned int inodo, void *buf, unsigned int offset, unsigned int nbytes) {
	
	int primer_byte = offset;
	int ultimo_byte = offset + nbytes - 1;
	int primer_blogico = offset/TB;
	int ultimo_blogico = ultimo_byte/TB;
	int aux = 0;
	int byte_ini = primer_byte%TB;
	int byte_fin = ultimo_byte%TB;
	int bytes_leidos = 0;
	int primer_ultimo = 0;	/* PROVISIONAL!!! PARA VER ERRORES */
	unsigned char buffer[TB];
	
	/* Valores calculados 
	printf("/-------------------------------------/\n");
	printf("	primer_byte 	= %d\n", primer_byte);
	printf("	ultimo_byte 	= %d\n", ultimo_byte);
	printf("	primer_blogico 	= %d\n", primer_blogico);
	printf("	ultimo_blogico 	= %d\n", ultimo_blogico);
	printf("/-------------------------------------/\n"); */
	
	
	int i;
	for (i = primer_blogico; i <= ultimo_blogico; i++) {
	
		//printf("Vamos a traducir_bloque_inodo(%d, %d, 0)\n", inodo, i);
		aux = traducir_bloque_inodo(inodo, i, 0);
		if (bread(aux, buffer) < 0) {
			printf("ERROR EN i==primer_blogico (mi_read_f): Error al bread(%d, buffer)\n", aux);
			return (-1);
		}
		//else
			//printf("Lectura correcta: bread(%d, buffer)\n", aux);
		
		/* REPASAR LOS memcpy!!! */
		if ((primer_blogico != ultimo_blogico) && (i == primer_blogico)) {
			//printf("- entra en primer_blogico con aux = %d\n", aux);
			//printf("	bread(%d, buffer)\n", aux);
			//printf("	byte_ini: %d\n",byte_ini);
			if (memcpy(buf, buffer + byte_ini, TB - byte_ini) < 0) {
				printf("ERROR EN PRIMERBLOGICO\n");
				return (-1);
			}
			//printf("	memcpy(%d, %d + %d, %d - %d)\n\n", buf, buffer, byte_ini, TB, byte_ini);
			primer_ultimo++;
		}
		else if ((primer_blogico == ultimo_blogico) || (i == ultimo_blogico)) {
			//printf("- entra en ultimo_blogico con aux = %d\n", aux);
			//printf("	byte_fin: %d\n",byte_fin);
			
			if (primer_blogico == ultimo_blogico) {
				memcpy(buf, buffer + byte_ini, byte_fin+1);
				//printf("	SOLO 1 BLOQUE memcpy(%d, %d+%d, %d)\n", buf, buffer, byte_ini, byte_fin);
			}
			else {
				if (memcpy(buf+(TB-byte_ini)+(ultimo_blogico-primer_blogico-1)*TB, buffer, byte_fin+1) < 0) {
					printf("ERROR EN ULTIMOBLOGICO\n");
					return (-1);
				}
				//printf("	memcpy(%d, %d + %d + %d, %d)\n\n", buffer, buf, (TB-byte_ini), (ultimo_blogico-primer_blogico-1)*TB, byte_fin+1);
				primer_ultimo++;
			}
		}
		else {
			//printf("- entra en el else con aux = %d\n", aux);
			if (memcpy(buf+(TB-byte_ini)+(i-primer_blogico-1)*TB, buffer, TB) < 0) {
				//printf("ERROR EN ELSE NUM %d\n", i);
				return (-1);
			}
			//printf("	memcpy(%d + %d + %d, %d, %d)\n\n", buf, (TB-byte_ini), (i-primer_blogico-1)*TB, buffer, TB);
			bytes_leidos++;
		}
	}
	
	//printf("(antes) bytes_leidos = %d (bloques del else) + %d (extremos)\n", bytes_leidos, primer_ultimo);
	
	if  (primer_blogico != ultimo_blogico) {
		bytes_leidos *= TB;
		bytes_leidos = bytes_leidos + (TB-byte_ini) + (byte_fin+1); /* cuando es 1 bloque, cuenta 1 de más */
	}
	else {
		bytes_leidos = (++byte_fin) - byte_ini;
	}
	
	//printf("(despues) bytes_leidos = %d (bloques del else)\n\n\n\n", bytes_leidos);
	
	return (bytes_leidos);	
}


int mi_truncar_f(unsigned int ninodo, unsigned int nbytes) {
	
	if (nbytes == 0) {
		if (liberar_inodo(ninodo) < 0) {
			printf("ERROR (ficheros.c -> mi_truncar_f (%d, %d)): Error al truncar a %d bytes (liberar_inodo)\n", ninodo, nbytes, nbytes);
			return (-1);
		}
	}
	else if (nbytes > 0) {
		if (liberar_bloques(ninodo, nbytes) < 0) {
			printf("ERROR (ficheros.c -> mi_truncar_f(%d, %d)): Error al truncar a %d bytes (liberar_bloques)\n", ninodo, nbytes, nbytes);
			return (-1);
		}
		
		/* Actualizamos el inodo */
		struct inodo in;
		if (leer_inodo(&in, ninodo) < 0) {
			printf("ERROR (ficheros.c -> mi_truncar_f(%d, %d)): Error al leer el inodo %d\n", ninodo, nbytes, ninodo);
			return (-1);
		}
		
		in.t_bytes = nbytes;
		in.f_modificacion = time(NULL);
		
		if (nbytes%TB > 0) {
			in.n_bloques = (nbytes%TB) + 1;
		}
		else {
			in.n_bloques = nbytes%TB;
		}
		
		if (escribir_inodo(in, ninodo) < 0) {
			printf("ERROR (ficheros.c -> mi_truncar_f(%d, %d)): Error al escribir el inodo %d\n", ninodo, nbytes, ninodo);
			return (-1);
		}
	}
	else {
		printf("Info (ficheros.c -> mi_truncar_f): El nº de bytes %d es menor que 0.\n", nbytes);
		return (-1);
	}
	
	return (1); /* Éxito en la ejecución */
}


int mi_stat_f(unsigned int ninodo, struct STAT *p_stat) {
	
	struct inodo in;
	if (leer_inodo(&in, ninodo) < 0) {
		printf("ERROR (ficheros.c -> mi_stat_f(%d)): Error al leer el inodo %d\n", ninodo, ninodo);
		return (-1);
	}
	
	p_stat->tipo = in.tipo;
	p_stat->t_bytes = in.t_bytes;
	p_stat->f_creacion = in.f_creacion;
	p_stat->f_modificacion = in.f_modificacion;
	p_stat->n_bloques = in.n_bloques;
	
	return (1);
}

