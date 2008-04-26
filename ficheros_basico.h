#ifndef _FICHEROS_BASICO_H
#define _FICHEROS_BASICO_H

#include <string.h>
#include <time.h>
#include "bloques.h"

/*
 * Funciones para la gestión de inodos, mapa de bits y superbloque.
 */

/*
 * Valor 1 para activar algún debug.
 */ 
#define DEBUG_FBASICO 0

/*
 * Tipos de inodos.
 */ 
#define LIBRE 1
#define DIRECTORIO 2
#define FICHERO 3

#define TAM_PDIR 8
#define TAM_PIND 3

#define DIV_INODOS 4

/*
 * Estructura para el superbloque, ocupa 1024 bytes.
 */
struct superbloque
{
	char nom_fs[16];
	unsigned int primerb_mb;
	unsigned int ultimob_mb;
	unsigned int primerb_ai;
	unsigned int ultimob_ai;
	unsigned int primerb_dt;
	unsigned int ultimob_dt;
	unsigned int ni_root;
	unsigned int ni_libre;
	unsigned int b_libres;
	unsigned int i_libres;
	unsigned int n_bloques;
	unsigned int n_inodos;
	char resto[960];
};


/*
 * Estructura para el inodo, ocupa 64 bytes.
 */
struct inodo
{
	unsigned int tipo;
	unsigned int t_bytes;
	time_t f_creacion;
	time_t f_modificacion;
	unsigned int n_bloques;
	unsigned int pb_dir[TAM_PDIR];
	unsigned int pb_ind[TAM_PIND];
};


/*
 * Funciones de consulta y apoyo.
 */
int leerSB();

/*
 * Funciones para calcular el tamaño en bloques.
 */
int tamMB(unsigned int nbloques);

int tamAI(unsigned int nbloques);


/*
 * Funciones para inicializar el sistema de ficheros.
 */
int initSB(unsigned int nbloques, char *nom_fs);

int initMB(unsigned int nbloques);

int initAI(unsigned int nbloques);


/*
 * Funciones de escritura y lectura del MB.
 */
int pbit(unsigned int nbloque);

int pbyte(unsigned int nbloque);

int escribir_bit(unsigned int nbit, char valor);

int leer_bit(unsigned int nbit);


/*
 * Funciones de gestión de bloques.
 */
int reservar_bloque();

int liberar_bloque(unsigned int nbloque);


/*
 * Funciones de gestión de inodos.
 */
int escribir_inodo(struct inodo in, unsigned int ninodo);

int leer_inodo(struct inodo *in, unsigned int ninodo);

int reservar_inodo(struct inodo in);

int liberar_bloques(unsigned int ninodo, unsigned int nbytes);

//FALTAN COSAS

#endif












