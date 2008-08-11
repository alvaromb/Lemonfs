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

int acabados = 0;



void reaper() {

	while(wait3(NULL, WNOHANG, NULL) > 0) {
		acabados++;
	}
	
	printf("Han acabado %d / %d\n", acabados, N_PROCESOS);
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
	
		//prt_stat_f(0);	
		//prt_stat_f(1);
		
		sprintf(linea, "Inicio log proceso PID %d\n", getpid());
		printf("Fichero <<%s>> creado! // %d // %s ", nombre, strlen(linea), linea);
		
		//printf("strlen(linea): %d\n	", strlen(linea));
		
		if (mi_write(nombre, linea, 0, strlen(linea)) < 0) {
			char buffer[10000];
			memset(buffer, '\0', 10000);
			mi_dir("/", buffer);
			printf("%s", buffer);
			printf("ERROR PERRO (simulacion.c): Error al ejecutar mi_write(%s, &s, 0, %d).\n", nombre, linea, strlen(linea));
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
						
			if (mi_stat(nombre, &estado) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_stat(%s, &estado) 1.\n", nombre);
				return (-1);
			}
			else {
				if (mi_write(nombre, linea, estado.t_bytes, strlen(linea)) < 0) {
					printf("ERROR (simulacion.c): Error al ejecutar mi_write(%s, linea, %d, %d) 1.\n", nombre, estado.t_bytes, strlen(linea));
					return (-1);
				}
			}
			
			usleep(100000); /* Cada línea debe esperar 0,1 segundos */
		}
		
		sprintf(linea, "Fin log proceso PID %d\n", getpid());
		
		printf("%s\n", linea);
		
		if (mi_stat(nombre, &estado) < 0) {
				printf("ERROR (simulacion.c): Error al ejecutar mi_stat(%s, &estado) 2.\n", nombre);
				return (-1);
		}
		else {
			if (mi_write(nombre, linea, estado.t_bytes, strlen(linea)) < 0) {
					printf("ERROR (simulacion.c): Error al ejecutar mi_write(%s, %s, %d, %d) 2.\n", nombre, linea,  estado.t_bytes, strlen(linea));
					return (-1);
			}
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
			printf("El proceso %s no existe.\n", nombre);
			//return (-1);
		}
		else {
			printf("%s eliminado!\n", nombre);
		}
	}
}



int main (int argc,char **argv) {

	if (argc == 2) {

		signal(SIGCHLD,reaper);
	
		mount(argv[1]);
		
		struct inodo in;
		if (leer_inodo(&in, 0) < 0) {
			printf("ERROR (simulacion.c -> mi_stat(/)): Error al leer el estado\n");
		}
		
		struct superbloque SB;
		if (bread(0, (char *)&SB) < 0) {
			printf("ERROR (simulacion.c -> error al leer el superbloque)\n");
		}
		
		if (in.t_bytes > 0) {
			vaciar();
			initSB(SB.n_bloques, argv[1]);
			initMB(SB.n_bloques);
			initAI(SB.n_bloques);
		}
		
		
		
		//prt_stat_f(0);
			
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

