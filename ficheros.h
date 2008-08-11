#ifndef _FICHEROS_H
#define _FICHEROS_H

#include <time.h>
#include "ficheros_basico.h"


typedef unsigned char BUFFER[TB];

/*
 * Estructura para los metadatos del inodo.
 */
struct STAT {
	unsigned int tipo;
	unsigned int t_bytes;
	time_t f_creacion;
	time_t f_modificacion;
	unsigned int n_bloques;
};

int mi_write_f(unsigned int inodo, const void *buf, unsigned int offset, unsigned int nbytes);

int mi_read_f(unsigned int inodo, void *buf, unsigned int offset, unsigned int nbytes);

int mi_truncar_f(unsigned int ninodo, unsigned int nbytes);

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat);

int prt_stat_f(unsigned int ninodo);

#endif

