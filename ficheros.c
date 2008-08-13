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
	unsigned char buffer[TB];
	
	const int cinodo = inodo;
	
	
	int i;
	for (i = primer_blogico; i <= ultimo_blogico; i++) {
	
		aux = traducir_bloque_inodo(inodo, i, 1);
		
		if ((primer_blogico != ultimo_blogico) && (i == primer_blogico)) {
			bread(aux, buffer);
			memcpy(buffer + byte_ini, buf, TB - byte_ini);
		}
		else if ((primer_blogico == ultimo_blogico) || (i == ultimo_blogico)) {
		
			bread(aux, buffer);
			
			if (primer_blogico == ultimo_blogico) {
				if (memcpy(buffer+byte_ini, buf, nbytes) < 0) {
				 	printf("ERROR (ficheros.c -> mi_write_f(%d, %s, %d, %d)): Error al ejecutar memcpy de pblogico == ublogico.\n", inodo, buf, offset, nbytes);
				 	return (-1);
				 }
			}
			else {
				memcpy(buffer, buf+(TB-byte_ini)+(ultimo_blogico-primer_blogico-1)*TB, byte_fin+1);
			}
		}
		else {
			memcpy(buffer, buf+(TB-byte_ini)+(i-primer_blogico-1)*TB, TB);
			bytes_escritos++;
		}
		
		bwrite(aux, buffer);
	}
	
	if (primer_blogico != ultimo_blogico) {
		bytes_escritos *=TB;
		bytes_escritos = bytes_escritos + (TB-byte_ini) + (byte_fin+1); /* en 1 o menos falla */
	}
	else {
		bytes_escritos = (++byte_fin) - byte_ini;
	}
	
	/* Actualizamos los datos del inodo */
	struct inodo in;
	if (leer_inodo(&in, cinodo) < 0) {
		printf("ERROR (ficheros.c -> mi_write_f(%d, buf, %d, %d)): Error al leer el inodo %d para actualizar sus datos.\n", inodo, offset, nbytes, inodo);
		return (-1);
	}
	
	in.f_modificacion = time(NULL);
	in.t_bytes = bytes_escritos+offset;
	
	if (in.t_bytes%TB > 0) {
		in.n_bloques = (in.t_bytes/TB) + 1;
	}
	else {
		in.n_bloques = in.t_bytes/TB;
	}
	
	if (escribir_inodo(in, cinodo) < 0) {
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
	unsigned char buffer[TB];	
	
	int i;
	for (i = primer_blogico; i <= ultimo_blogico; i++) {
	
		aux = traducir_bloque_inodo(inodo, i, 0);
		if (bread(aux, buffer) < 0) {
			printf("ERROR EN i==primer_blogico (mi_read_f): Error al bread(%d, buffer)\n", aux);
			return (-1);
		}
		
		if ((primer_blogico != ultimo_blogico) && (i == primer_blogico)) {
		
			if (memcpy(buf, buffer + byte_ini, TB - byte_ini) < 0) {
				printf("ERROR EN PRIMERBLOGICO\n");
				return (-1);
			}
		}
		else if ((primer_blogico == ultimo_blogico) || (i == ultimo_blogico)) {
			
			if (primer_blogico == ultimo_blogico) {
				memcpy(buf, buffer + byte_ini, nbytes);
			}
			else {
				if (memcpy(buf+(TB-byte_ini)+(ultimo_blogico-primer_blogico-1)*TB, buffer, byte_fin+1) < 0) {
					printf("ERROR EN ULTIMOBLOGICO\n");
					return (-1);
				}
			}
		}
		else {
			if (memcpy(buf+(TB-byte_ini)+(i-primer_blogico-1)*TB, buffer, TB) < 0) {
				printf("ERROR EN ELSE NUM %d\n", i);
				return (-1);
			}
			bytes_leidos++;
		}
	}
	
	if  (primer_blogico != ultimo_blogico) {
		bytes_leidos *= TB;
		bytes_leidos = bytes_leidos + (TB-byte_ini) + (byte_fin+1); /* cuando es 1 bloque, cuenta 1 de más */
	}
	else {
		bytes_leidos = (++byte_fin) - byte_ini;
	}
	
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


int prt_stat_f(unsigned int ninodo) {

	struct STAT p_stat;
	if (mi_stat_f(ninodo, &p_stat) < 0) {
		printf("ERROR (ficheros.c -> prt_stat_f(%d)): Error al leer el estado del inodo %d\n", ninodo, ninodo);
		return (-1);
	}
	
	printf(" --- mi_stat_f(%d) -----------------\n\n", ninodo);
	printf(" p_stat.tipo           = ");
	if (p_stat.tipo == LIBRE) {
		printf("LIBRE\n");
	}
	else if (p_stat.tipo == DIRECTORIO) {
		printf("DIRECTORIO\n");
	}
	else if (p_stat.tipo == FICHERO) {
		printf("FICHERO\n");
	}
	printf(" p_stat.t_bytes        = %d\n", p_stat.t_bytes);
	printf(" p_stat.f_creacion     = %d\n", p_stat.f_creacion);
	printf(" p_stat.f_modificacion = %d\n", p_stat.f_modificacion);
	printf(" p_stat.n_bloques      = %d\n", p_stat.n_bloques);
	printf(" -----------------------------------\n\n");
	
	return (1);
}

