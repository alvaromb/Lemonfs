#ifndef _FICHEROS_H
#define _FICHEROS_H

#include <time.h>
#include "ficheros_basico.h"

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


#endif