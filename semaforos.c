/* Libreria "semaforos" 
	semaforos.c	*/


//mi_write, resrevar bloque, resevrar inodo

//Librerias del Sistema utilizadas.
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>


//Función encargada de crear un nuevo semáforo.
int nuevo_semaforo(int llave, int num){

	int s = semget(llave, num, IPC_CREAT|0600);

	if(s<0){
		printf("Error creando semáforo \n");
		exit(0);
	}

	return s;
}


//Función encargada de esperar un semáforo.
void esperar_semaforo(int s, int pos, int flag){

	struct sembuf sbuf;

	sbuf.sem_num = pos;
	sbuf.sem_op = -1;
	sbuf.sem_flg = flag;
	semop(s, &sbuf, 1);

}


//Función encargada en señalizar un semáforo.
void senalizar_semaforo(int s, int pos){

	struct sembuf sbuf;

	sbuf.sem_num = pos;
	sbuf.sem_op = 1;
	sbuf.sem_flg = 0;
	semop(s, &sbuf, 1);

}


//Función encargada en inicializar un semáforo.
void inicializar_semaforo(int s, int valor){

	semctl(s, 0, SETVAL, valor);

}


//Función encargada de eliminar un semáforo.
void eliminar_semaforo(int s){

	int rem = semctl(s, 0, IPC_RMID, 0);

	if(rem<0){ 
		printf("Error eliminando semáforo %d...\n", s);
		exit(0);
	}

	printf("Eliminado semáforo #%d\n\n", s);
}


