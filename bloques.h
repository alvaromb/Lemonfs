#ifndef _BLOQUES_H
#define _BLOQUES_H

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * Valor 1 para activar algún debug.
 */
#define DEBUG 0
#define DEBUG_BLOQUES 0

/*
 *	Tamaño de los bloques.
 */
#define TB 1024
#define TP 256


int bmount(const char *camino);

int bumount();

int bwrite(unsigned int bloque, const void *buf);

int bread(unsigned int bloque, void *buf);

#endif
