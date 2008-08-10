#ifndef _SEMAFOROS_H
#define _SEMAFOROS_H

/* Cabecera de la libreria "Semaforos" 
		semaforos.h 		*/


//Funciones
int nuevo_semaforo(int llave, int num);
void esperar_semaforo(int s, int pos, int flag);
void senalizar_semaforo(int s, int pos);
void inicializar_semaforo(int s, int valor);
void eliminar_semaforo(int s);

#endif

