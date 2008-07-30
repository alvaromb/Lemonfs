#ifndef _DIRECTORIOS_H
#define _DIRECTORIOS_H

#include "ficheros_basico.h"
#include "ficheros.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TAM_NOMDIR 60


/*
 * Datos para nombrar y asociar directorios con inodos.
 */
struct entrada {
	char nombre[TAM_NOMDIR];
	unsigned int inodo;
};


int extraer_camino(const char *camino, char *inicial, char *final);

int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada);

int extraer_directorio(const char *camino, char *inicial);

int mi_creat(const char *camino);

#endif
