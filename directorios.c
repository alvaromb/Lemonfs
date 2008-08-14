#include "directorios.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "semaforos.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Semáforo */
int mutex;


int mount(const char *nombre) {
	
	if (bmount(nombre) < 0) {
		printf("ERROR (directorios.c ->): METER EL ERROR\n");
		return (-1);
	}
	else {
		mutex = nuevo_semaforo(ftok("/home/", 's'), 1);
		inicializar_semaforo(mutex, 1);
		
		return (1);
	}
}



int unmount(const char *nombre) {

	bumount();
	eliminar_semaforo(mutex);
	
	return (1);
}



int extraer_camino(const char *camino, char *inicial, char *final) {
	
	int longitud = strlen(camino);
		
	if (longitud > 0) {
	
		int i = 1; /* Saltamos el carácter '/' */
		while ((camino[i] != '\0') && (camino[i] != '/') && (i < longitud)) {
			inicial[i-1] = camino [i];
			i++;
		}
		
		inicial[i-1] = '\0';
		
		int j = 0;
		while (camino[i] != '\0') {
			final[j] = camino[i];
			i++;
			j++;
		}
		
		final[j] = '\0';
		
		return (1); /* Ejecución finalizada con éxito */
	}
	else {
	
		return (-1);
	}		
}


int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada) {

	char inicial[TAM_NOMDIR];
	memset(inicial, '\0', TAM_NOMDIR);
	char final[TAM_NOMDIR];
	memset(final, '\0', TAM_NOMDIR);
	int encontrada = -1;
	
	if (extraer_camino(camino_parcial, inicial, final) < 0) {
		printf("ERROR (directorios.c -> buscar_entrada(%s, %d, %d, %d)): Error al extraer el camino %s.\n", camino_parcial, *p_inodo_dir, *p_inodo, *p_entrada, camino_parcial);
		return (-1);
	}
	
	struct STAT estado;
	if (mi_stat_f(*p_inodo_dir, &estado) < 0) {
		printf("ERROR (directorios.c -> buscar_entrada()): Error al ejecutar mi_stat_f(%d).\n", *p_inodo_dir);
		return (-1);
	}
	
	struct superbloque sb;
	if (bread(0, (char *)&sb) < 0) {
		printf("ERROR (directorios.c -> buscar_entrada()): Error al leer el superbloque.\n");
		return (-1);
	}
	
	int n_entradas = (estado.t_bytes/sizeof(struct entrada));
	
	/* Comprobamos que el inodo sea directorio y que contenga entradas */
	if ((estado.tipo == DIRECTORIO) && (n_entradas > 0) && (*p_inodo_dir < sb.n_inodos) && (strcmp(camino_parcial, "/") != 0)) {
	
		int i = 0;
		struct entrada ent[n_entradas];
		int inodo = *p_inodo_dir;
		
		if (mi_read_f(*p_inodo_dir, &ent, 0, estado.t_bytes) < 0) {
				printf("ERROR (directorios.c -> buscar_entrada // mi_read_f(%d, &ent, %d*64, 64)): Error al leer la entrada con i = %d\n", *p_inodo_dir, i, i);
				return (-1);
		}
		
		while ((encontrada < 0) && (inodo >= 0) && (i < n_entradas)) {
						
			if (strcmp(inicial, ent[i].nombre) == 0) {
				encontrada = 1;
			}
			else {
				i++;
			}
		}
		
		/* Hemos hallado la entrada */
		if (encontrada > 0) {
			/* Caso básico */
			if (((final[0] == '/') && (strlen(final) == 1)) || (final[0] == '\0')) {
				*p_inodo = ent[i].inodo;
				*p_entrada = i;
				encontrada = 1;
			}
			
			/* Caso general */
			else {
				*p_inodo_dir = ent[i].inodo;
				encontrada = buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada);
			}
		}
		
		/* No existe la entrada en el inodo */
		else {
			*p_entrada = i;			
			encontrada = -1;
		}
	}
	
	/* Inodo raíz */
	else if (strcmp(camino_parcial, "/") == 0) {
		*p_inodo_dir = 0;
		encontrada = 1;
	}
	else {
		encontrada = -1;
	}
	
	return (encontrada);
}


int extraer_directorio(const char *camino, char *inicial, char *nombre) {

	if ((camino[0] == '/') && (strcmp(camino, "/") != 0)) {
	
		int longitud = strlen(camino);
		
		int i;
		if (camino[longitud-1] == '/') {
			i = longitud-2;
		}
		else {
			i = longitud-1;
		}
		
		while (camino[i] != '/') {
			i--;
		}
		
		int p = i;
		i = 0;
		while (i <= p) {
			inicial[i] = camino[i];
			i++;
		}
	
		int j = 0;
		while (camino[i] != '/' && i < longitud) {
			nombre[j] = camino[i];
			j++;
			i++;	
		}
	
		nombre[j] = '\0';
		
		return (1);	
	}
	else {
		printf("ERROR (directorios.c -> extraer_directorio(%s)): Camino no válido.\n", camino);
		return (-1);
	}
}



int mi_creat(const char *camino) {

	esperar_semaforo(mutex, 0, 0);

	int longitud = strlen(camino);
	int devolver = -1;

	if ((longitud > 0) && (strcmp(camino, "/") != 0)) {
	
		struct entrada ent;
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo;
		unsigned int p_entrada;
		
		/* Comprobamos que exista el camino */
		char inicial[TAM_NOMDIR];
		memset(inicial, '\0', TAM_NOMDIR);
		char final[TAM_NOMDIR];
		memset(final, '\0', TAM_NOMDIR);
				
		if (extraer_directorio(camino, inicial, final) < 0) {
			printf("ERROR (directorios.c -> mi_creat(%s)): Error al extraer el directorio.\n", camino);
			devolver = -1;
		}
		else {
		
			p_inodo_dir = 0;
			p_inodo = 0;
			p_entrada = 0;
			
			if (buscar_entrada(inicial, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
				/* No existe el camino, ERROR */
				printf("mi_creat: no se puede crear el directorio <<%s>>: No existe el fichero ó directorio\n", camino);
				devolver = -1;
			}
			else {
				/* Existe el camino, buscamos que no exista la entrada */
				p_inodo_dir = 0;
				p_inodo = 0;
				p_entrada = 0;
				
				if (buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
					
					struct inodo in;
					
					/* Es del tipo DIRECTORIO */
					if (camino[longitud-1] == '/') {
						in.tipo = DIRECTORIO;
					}
					/* Es del tipo FICHERO */
					else {
						in.tipo = FICHERO;
					}
					
					in.t_bytes = 0;
					in.n_bloques = 0;
					in.f_creacion = time(NULL);
					in.f_modificacion = time(NULL);
					int inodo_r = reservar_inodo(in);
					
					/* Escribimos el Fichero/directorio */
					struct STAT estado;
					if (mi_stat_f(p_inodo_dir, &estado) < 0) {
						printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer el estado del inodo %d.\n", camino, p_inodo_dir);
						devolver = -1;
					}
					else {
					
						int n_entradas = (estado.t_bytes/sizeof(struct entrada));
						struct entrada ent[n_entradas+1];
						
						if (n_entradas > 0) {
						
							if (mi_read_f(p_inodo_dir, &ent, 0, estado.t_bytes) < 0) {
								printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer las %d entradas del inodo %d.\n ", camino, n_entradas, p_inodo_dir);
								devolver = -1;
							}
						}
						
						/* Introducimos los datos en la entrada */
						ent[n_entradas].inodo = inodo_r;
						strcpy(ent[n_entradas].nombre, final);
						
						/* Escribimos la entrada */
						if (mi_write_f(p_inodo_dir, &ent, 0, ((n_entradas+1)*sizeof(struct entrada))) < 0) {
							printf("ERROR (directorios.c -> mi_creat(%s)): Error al escribir la entrada nueva.\n", camino);
							devolver = -1;
						}
						else {
							devolver = 1;
						}
					}
				}
				/* Existe la entrada, no podemos crearla */
				else {
					printf("mi_creat: no se puede crear la entrada <<%s>>: El fichero ya existe\n", final);
					devolver = -1;
				}
			}
		}
	}
	
	senalizar_semaforo(mutex, 0);
	return (devolver);
}



int mi_unlink(const char *camino) {

	esperar_semaforo(mutex, 0, 0);
	
	int longitud = strlen(camino);
	int devolver = -1;	
	int n_entradas = 0;
	
	if ((longitud > 0) && (strcmp(camino, "/") != 0)) {
		
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo;
		unsigned int p_entrada;
		
		/* Comprobamos que exista la entrada */	
		if (buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
			/* No existe la entrada, ERROR */
			printf("mi_unlink: no se puede borrar <<%s>>: No existe el fichero ó directorio\n", camino);
			devolver = -1;
		}
		else {
		
			/* Existe la entrada, podemos borrarla */
			struct STAT estado;
			if (mi_stat_f(p_inodo_dir, &estado) < 0) {
				printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer el estado del inodo %d.\n", camino, p_inodo_dir);
				devolver = -1;
			}
			
			else {
			
				n_entradas = (estado.t_bytes/sizeof(struct entrada));
				struct entrada ent[n_entradas];
				
				if (mi_read_f(p_inodo_dir, &ent, 0, ((n_entradas)*sizeof(struct entrada))) < 0) {
					printf("ERROR (directorios.c -> mi_unlink(%s)): Error al leer las %d entradas del inodo %d.\n", camino, ((n_entradas)*sizeof(struct inodo)), p_inodo_dir);
					devolver = -1;
				}				
				else {
				
					if (estado.t_bytes > sizeof(struct entrada)) {
					
						/* Guardamos la última entrada y re-localizamos */
						struct entrada pivote;
						memset(pivote.nombre, '\0', TAM_NOMDIR);
						strcpy(pivote.nombre, ent[n_entradas-1].nombre);
						
						pivote.inodo = ent[n_entradas-1].inodo;
						strcpy(ent[p_entrada].nombre, pivote.nombre);
						ent[p_entrada].inodo = pivote.inodo;
						
						if (mi_write_f(p_inodo_dir, &ent, 0, ((n_entradas-1)*sizeof(struct entrada))) < 0) {
							printf("ERROR (directorios.c -> mi_unlink(%s)): Error al escribir la nueva localización de la entrada en la posición %d\n", camino, p_entrada);
							devolver = -1;
						}
					}
					
					/* Actualizamos el inodo del directorio donde estaba el fichero/directorio borrado. 
					 * Ocurre cuando sólo hay una entrada en dicho directorio y no se actualiza mediante mi_write_f
					 */
					else {
					
						struct inodo dir;
						if (leer_inodo(&dir, p_inodo_dir) < 0) {
							printf("ERROR (directorios.c -> mi_unlink(%s)): Error al leer el inodo %d para actualizarlo.\n", camino, p_inodo_dir);
							devolver = -1;
						}
						else {
							
							dir.t_bytes -= 64;
							dir.n_bloques -= 1;
							dir.f_modificacion = time(NULL);
							
							if (escribir_inodo(dir, p_inodo_dir) < 0) {
								printf("ERROR (directorios.c -> mi_unlink(%s)): Error al escribir el inodo %d para actualizarlo.\n", camino, p_inodo_dir);
								devolver = -1;
							}
						}
					}
				}
			}
			
			/* Debemos liberar el inodo */
			if (liberar_inodo(p_inodo) < 0) {
				printf("ERROR (directorios.c -> mi_unlink(%s)): Error al liberar el inodo %d.\n", camino, p_inodo);
				devolver = -1;
			}
			else {
			
				/* Truncamos el inodo que contiene p_inodo */
				if (n_entradas > 0) {
					n_entradas--;
				}
				
				if (mi_truncar_f(p_inodo_dir, (n_entradas*sizeof(struct entrada))) < 0) {
					printf("ERROR (directorios.c -> mi_unlink(%s)): Error al truncar el inodo %d en %d bytes.\n", p_inodo_dir, n_entradas*sizeof(struct entrada));
					devolver = -1;
				}
				else {
					devolver = 1;
				}
			}
		}
	}
	else if (strcmp(camino, "/") == 0) {
		printf("mi_unlink: Error al borrar el directorio <</>>: No se puede eliminar el directorio raíz\n");
		devolver = -1;
	}
	
	senalizar_semaforo(mutex, 0);
	return (devolver);
}



int mi_dir(const char *camino, char *buffer) {

	int longitud = strlen(camino);
	
	if (longitud > 0) {
	
		if (camino[longitud-1] == '/') {
			
			unsigned int p_inodo_dir = 0;
			unsigned int p_inodo;
			unsigned int p_entrada;
			
			int cacuna = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada);
			
			if (cacuna < 0) {
				printf("mi_dir: no se puede listar <<%s>>: No existe el directorio\n", camino);
				return (-1);
			}
			else {
			
				struct STAT estado;
				if (mi_stat_f(p_inodo_dir, &estado) < 0) {
					printf("ERROR (directorios.c -> mi_dir(%s, buffer)): Error al leer el estado del inodo %d.\n", camino, p_inodo_dir);
					return (-1);
				}
				
				int n_entradas = (estado.t_bytes/sizeof(struct entrada));
				
				if (n_entradas > 0) {
					
					struct entrada ent[n_entradas];
					if (mi_read_f(p_inodo_dir, &ent, 0, estado.t_bytes) < 0) {
						printf("ERROR (directorios.c -> mi_dir(%s, buffer)): Error al leer las %d entradas del inodo %d.\n", camino, n_entradas, p_inodo);
						return (-1);
					}
					
					int i;
					for (i = 0; i < n_entradas; i++) {
					
						strcat(buffer, "   ");
						strcat(buffer, ent[i].nombre);
						
						struct inodo in;
						leer_inodo(&in, ent[i].inodo);
						if (in.tipo == DIRECTORIO) {
							strcat(buffer, "/");
						}
						else {
							strcat(buffer, "  t_bytes = ");
							char aux[10];
							memset(aux, '\0', 10);
							sprintf(aux, "%d", in.t_bytes);
							strcat(buffer, aux);
						}
						
						strcat(buffer, ":\n");
					}
					
					/* Devolvemos el nº de entradas */
					return (n_entradas);
				}
				else {
					printf("mi_dir: el directorio <<%s>> está vacío\n", camino);
				}
			}
		}
		else {
			printf("mi_dir: no se puede listar <<%s>>: No es un directorio\n", camino);
			return (-1);
		}
	}
}



int mi_stat(const char *camino, struct STAT *p_stat) {

	esperar_semaforo(mutex, 0, 0);

	int devolver = -1;
	int longitud = strlen(camino);
	
	if (longitud > 0) {
		
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo;
		unsigned int p_entrada;
		
		if (buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
			/* No existe el fichero/directorio */
			printf("mi_stat: no se puede mostrar el estado de <<%s>>: No existe el fichero ó directorio\n", camino);
			devolver = -1;
		}
		else {
		
			if (mi_stat_f(p_inodo, p_stat) < 0) {
				printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer el estado del inodo %d.\n", camino, p_inodo_dir);
				devolver = -1;
			}
			else {
				devolver = 1; /* Ejecución finalizada con éxito */
			}
		}
	}
	
	senalizar_semaforo(mutex, 0);
	return (devolver);
}



int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes) {

	esperar_semaforo(mutex, 0, 0);
	
	int devolver = -1;
	int longitud = strlen(camino);
	
	if (longitud > 0) {
		
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo = 0;
		unsigned int p_entrada = 0;
		
		if (buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
			printf("mi_write: no se puede escribir el búfer en <<%s>>: No existe el fichero ó directorio\n", camino);
			devolver = -1;
		}
		else {
		
			if (mi_write_f(p_inodo, buf, offset, nbytes) < 0) {
				printf("ERROR (directorios.c -> mi_write(%s, buf, %d, %d)): Error al ejecutar mi_write_f(%d, buf, %d, %d).\n", camino, offset, nbytes, p_inodo, offset, nbytes);
				devolver = -1;
			}
			else {
				devolver = 1; /* Ejecución finalizada con éxito */
			}
		}
	}
	
	senalizar_semaforo(mutex, 0);
	return (devolver);
}



int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes) {

	int longitud = strlen(camino);
	
	if (longitud > 0) {
		
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo;
		unsigned int p_entrada;
		
		if (buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
			printf("mi_read: no se puede leer en <<%s>>: No existe el fichero ó directorio\n", camino);
			return (-1);
		}
		else {
		
			if (mi_read_f(p_inodo, buf, offset, nbytes) < 0) {
				printf("ERROR (directorios.c -> mi_read(%s, buf, %d, %d)): Error al ejecutar mi_read(%d, buf, %d, %d).\n", camino, offset, nbytes, p_inodo, offset, nbytes);
				return (-1);
			}
			
			return (1); /* Ejecución finalizada con éxito */
		}
	}
}















