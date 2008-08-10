#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "ficheros_basico.h"
#include "directorios.h"

#define N_PROCESOS 100
#define N_LOGS 100
#define N_LINEA 50

static int acabados;



void reaper() {

	while(wait3(NULL,WNOHANG,NULL) > 0) {
		acabados++;
	}
}



int proceso (int n) {
	
	char nombre[N_LINEA];
	memset(nombre, '\0', N_LINEA);
	char linea[N_LINEA];
	memset(linea, '\0', N_LINEA);

	sprintf(nombre,"/proceso-%d.dat",n);
	printf("%s\n", nombre);
	
	if (mi_creat(nombre) < 0) {
		printf("ERROR (simulacion.c): Error al ejecutar mi_creat(%s).\n", nombre);
		return (-1);
	}
	else {
	
		sprintf(linea, "Inicio log proceso PID %d\n", getpid());
		printf("Fichero <<%s>> creado! // %s\n", nombre, linea);
		
		/* PRUEBAS */
		unsigned int p_inodo_dir, p_inodo, p_entrada;
		p_inodo_dir = 0;
		int res = buscar_entrada(nombre, &p_inodo_dir, &p_inodo, &p_entrada);
		
		//printf("Linea: %s, RES: %d, p_inodo_dir = %d, p_inodo = %d, p_entrada = %d.\n", linea, res, p_inodo_dir, p_inodo, p_entrada);
		
		
		if (mi_write(nombre, linea, 0, strlen(linea)) < 0) {
			printf("ERROR (simulacion.c): Error al ejecutar mi_write(%s, &s, 0, %d).\n", nombre, linea, strlen(linea));
			return (-1);
		}
		
		/* Para imprir la hora */
		struct tm *p_tiempo;
		time_t tiempo;
		struct STAT estado;
		int i;
		for (i = 0; i < N_LOGS; i++) {
			tiempo = time(NULL);
			p_tiempo = localtime(&tiempo);
			sprintf(linea, "  %d:%d:%d Línea número %d\n", p_tiempo->tm_hour, p_tiempo->tm_min, p_tiempo->tm_sec, i);
						
			//printf("%s\n", linea);
						
			if (mi_stat(nombre, &estado) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_stat(%s, &estado) 1.\n", nombre);
				return (-1);
			}
			
			//printf("estado.t_bytes = %d\n", estado.t_bytes);
			
			if (mi_write(nombre, linea, estado.t_bytes, strlen(linea)) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_write(%s, linea, %d, %d) 1.\n", nombre, estado.t_bytes, strlen(linea));
				return (-1);
			}
			
			usleep(100000); /* Cada línea debe esperar 0,1 segundos */
		}
		
		sprintf(linea, "Fin log proceso PID %d\n", getpid());
		
		printf("%s\n", linea);
		
		if (mi_stat(nombre, &estado) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_stat(%s, &estado) 2.\n", nombre);
				return (-1);
		}
		
		if (mi_write(nombre, linea, estado.t_bytes, strlen(linea)) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_write(%s, %s, %d, %d) 2.\n", nombre, linea,  estado.t_bytes, strlen(linea));
				return (-1);
		}
	}
	
	return (1);
}


/* ADAPTARLA PARA QUE BORRE LO QUE TOCA!!! */
int vaciar() {
	
	char nombre[N_LINEA];
	memset(nombre, '\0', N_LINEA);

	int i;
	for(i = 0; i < N_PROCESOS; i++) {
		sprintf(nombre,"/proceso-%d.dat",i);

		if (mi_unlink(nombre) < 0) {
			printf("ERROR (simulacion.c): Error al ejecutar mi_unlink(%s).\n", nombre);
			return (-1);
		}
		else {
			printf("%s eliminado!\n", nombre);
		}
	}
}



int main (int argc,char **argv) {

	if (argc == 2) {

		signal(SIGCHLD,reaper);
		acabados = 0;		
	
		mount(argv[1]);
		
		
		vaciar(); /* METER CONTROL DE ERRORES */
			
			int i;
			for (i = 0; i < N_PROCESOS; i++) {
			
				int hilo = fork();
				if (hilo == 0) {
				
					if (proceso(i) < 0) {
						printf("ERROR (simulacion.c): Error al crear el proceso %d.\n", i);
						return (-1);
					}
					exit(0);
				}
				else if (hilo < 0) {
					i--;
					printf("Llamamos al reaper, hilo = %d\n", hilo);
					reaper();
				}
				
				sleep(1);		
			}
		
			while (acabados < N_PROCESOS) {
				pause();
			}
			
		unmount(argv[1]);
	
	}
	else {
		printf("ERROR (simulacion.c): Error, parámetros != de 2 (%d).\n", argc);
		return (-1);
	}
	
	return (0);
}

