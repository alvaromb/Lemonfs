#ifndef _DIRECTORIOS_H
#define _DIRECTORIOS_H

#include "directorios.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "semaforos.h"
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


/*
 * Funciones para montar el sistema de ficheros y inicializar los semáforos
 * para la simulación.
 */
int mount(const char *nombre);

int unmount(const char *nombre);


/*
 * Funciones de soporte para buscar entrada y extraer directorios/caminos
 */
int extraer_camino(const char *camino, char *inicial, char *final);

int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada);

int extraer_directorio(const char *camino, char *inicial, char *nombre);


/*
 * Funciones para crear, borrar, listar, escribir y conocer el estado
 * de las entradas.
 */
int mi_creat(const char *camino);

int mi_unlink(const char *camino);

int mi_dir(const char *camino, char *buffer);

int mi_stat(const char *camino, struct STAT *p_stat);

int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes);

int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes);

#endif
