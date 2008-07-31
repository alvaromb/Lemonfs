#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final) {

	int longitud = strlen(camino);
	
	int i = 1; /* Saltamos el carácter '/' */
	while ((camino[i] != '\0') && (camino[i] != '/') && (i < longitud)) {
		inicial[i-1] = camino [i];
		i++;
	}
	
	inicial[i-1] = '\0';
	
	int j = 0;
	while ((camino[i] != '\0') && (i < longitud)) {
		final[j] = camino[i];
		i++;
		j++;
	}
	
	final[j] = '\0';
	
	//printf("     EXTRAER_CAMINO: inicial (%s, long = %d) // final (%s, long = %d)\n", inicial, strlen(inicial), final, strlen(final));
	
	if (final[0] == '/') {
		return (DIRECTORIO);
	}
	else {
		return (FICHERO);
	}
}


int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir, unsigned int *p_inodo, unsigned int *p_entrada) {

	char *inicial;
	char *final;
	char *final1;
	char *final2;
	inicial = (char *)malloc(sizeof(char *));
	final = (char *)malloc(sizeof(char *));
	final1 = (char *)malloc(sizeof(char *));
	final1[0] = '/';
	final2 = (char *)malloc(sizeof(char *));
	final2[0] = '\0';
	
	extraer_camino(camino_parcial, inicial, final);
	
	/* PRINTS AQUI 
	printf("\n - Valores iniciales\n");
	printf("    camino_parcial : %s     longitud: %d\n", camino_parcial, strlen(camino_parcial));
	printf("    inicial        : %s                   longitud: %d\n", inicial, strlen(inicial));
	printf("    final          : %s          longitud: %d\n", final, strlen(final));
	printf("    *p_inodo_dir   : %d\n", *p_inodo_dir);
	printf("    *p_inodo       : %d\n", *p_inodo);
	printf("    *p_entrada     : %d\n\n", *p_entrada);
	/* FIN PRINTS */
	
	//printf(" - Vamos a buscar %s en el inodo\n", inicial);
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
	//printf(" - n_entradas = %d\n", n_entradas);
	
	/* Comprobamos que el inodo sea directorio y que contenga entradas */
	if ((estado.tipo == DIRECTORIO) && (n_entradas > 0) && (*p_inodo_dir < sb.n_inodos)) {
	
		int i = 0;
		int encontrada = -1;
		struct entrada ent[n_entradas];
		int inodo = *p_inodo_dir;
		
		if (mi_read_f(*p_inodo_dir, &ent, 0, estado.t_bytes) < 0) {
				printf("ERROR (directorios.c -> mi_read_f(%d, &ent, %d*64, 64)): Error al leer la entrada con i = %d\n", *p_inodo_dir, i, i);
				return (-1);
		}
		
		while ((encontrada < 0) && (inodo >= 0)) {
			//printf("   - Bucle con: i = %d, encontrada = %d\n", i, encontrada);			
			
			char *c1;
			c1 = (char *)malloc(sizeof(char *));
			char *c2;
			c2 = (char *)malloc(sizeof(char *));
			
			strcpy(c2, ent[i].nombre);
			
			while ((strcmp(c2, "/") != 0) && (strcmp(c2, "\0") != 0)) {
				extraer_camino(c2, c1, c2);
				//printf("        c2 coño: %s // strcmp(/): %d strcmp(b0): %d\n", c2, strcmp(c2, "/"), strcmp(c2, "\0"));
			}
			
			//printf("     - Valor leído de la entrada: %s y su longitud: %d (inodo: %d)\n", ent[i].nombre, strlen(ent[i].nombre), ent[i].inodo, c1);
			//printf("     - Valor de inicial: %s y su longitud: %d // c1 = %s // c2 = %s\n", inicial, strlen(inicial), c1, c2);
			
			if (strcmp(inicial, c1) == 0) {
				//printf("     - Encontrada!!! ent.nombre: %s // ent.inodo: %d\n", ent[i].nombre, ent[i].inodo);
				encontrada = 1;
			}
			else {
				i++;
			}
		}
		
		/* Hemos hallado la entrada */
		if (encontrada > 0) {
			/* Caso básico */
			//if (((strcmp(final, final1)) == 0) || ((strcmp(final, final2)) == 0)) {
			if (((final[0] == '/') && (strlen(final) == 1)) || (final[0] == '\0')) {
				//printf("    Caso final\n");
				*p_inodo = ent[i].inodo;
				*p_entrada = i;
			}
			
				
			/* Caso general */
			else {
				//printf("    Caso general con final %s, y longitud %d y ent.inodo %d\n\n", final, strlen(final), ent[i].inodo);
				*p_inodo_dir = ent[i].inodo;
				buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada);
			}
			
			//printf("-------------------------------\n");
			
			return (1); /* Ejecución terminada con éxito */
		}
		
		/* No existe la entrada en el inodo */
		else {
			*p_entrada = i;
			//printf("Info (directorios.c -> buscar_entrada): La entrada %s no existe, devolvemos la última entrada.\n", camino_parcial);
			
			return (-1);
		}
	}
	
	/* Inodo raíz */
	else if (strcmp(camino_parcial, "/") == 0) {
		
		//printf("   Es inodo raíz!!!\n");
		*p_inodo_dir = 0;
		return (1);
	}
	else {
		//printf("   No hay entradas.\n");
		return (-1); 
	}
}


int extraer_directorio(const char *camino, char *inicial) {

	if (camino[0] == '/') {
	
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
		
		longitud = i;
		i = 0;
		while (i <= longitud) {
			inicial[i] = camino[i];
			i++;
		}
		
		return (1);	
	}
	else {
		printf("ERROR (directorios.c -> extraer_directorio(%s)): Camino no válido.\n", camino);
		return (-1);
	}
}


int mi_creat(const char *camino) {

	int longitud = strlen(camino);

	if ((longitud > 0) && (strcmp(camino, "/") != 0)) {
	
		struct entrada ent;
		unsigned int p_inodo_dir = 0;
		unsigned int p_inodo;
		unsigned int p_entrada;
		
		/* Comprobamos que exista el camino */
		char *inicial;
		inicial = malloc(sizeof(char *));
		if (extraer_directorio(camino, inicial) < 0) {
			printf("ERROR (directorios.c -> mi_creat(%s)): Error al extraer el directorio.\n", camino);
			return (-1);
		}
		
		p_inodo_dir = 0;
		p_inodo = 0;
		p_entrada = 0;
		printf(" - Parámetros de buscar el camino: inicial = %s, p_inodo_dir = %d, p_inodo = %d, p_entrada = %d\n", inicial, p_inodo_dir, p_inodo, p_entrada);
		
		if (buscar_entrada(inicial, &p_inodo_dir, &p_inodo, &p_entrada) < 0) {
			/* No existe el camino, ERROR */
			printf("mi_creat: no se puede crear el directorio <<%s>>: No existe el fichero ó directorio\n", camino);
			printf("DEBUG: inicial = %s\n", inicial);
			return (-1);
		}
		
		else {
			/* Existe el camino, buscamos que no exista la entrada */
			p_inodo_dir = 0;
			p_inodo = 0;
			p_entrada = 0;
			printf(" - Parámetros de buscar la entrada: camino = %s, p_inodo_dir = %d, p_inodo = %d, p_entrada = %d\n", camino, p_inodo_dir, p_inodo, p_entrada);
			int mierda = buscar_entrada(camino, &p_inodo_dir, &p_inodo, &p_entrada);
			printf(" - mierda = %d\n", mierda);
			if (mierda < 0) {
				
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
				
				/* FINALMENTE ESCRIBIMOS EL FICHERO/DIRECTORIO */
				struct STAT estado;
				if (mi_stat_f(p_inodo_dir, &estado) < 0) {
					printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer el estado del inodo %d.\n", camino, p_inodo_dir);
					return (-1);
				}
				
				int n_entradas = (estado.t_bytes/sizeof(struct entrada));
				struct entrada ent[n_entradas+1];
				
				if (n_entradas > 0) {
					if (mi_read_f(p_inodo_dir, &ent, 0, estado.t_bytes) < 0) {
						printf("ERROR (directorios.c -> mi_creat(%s)): Error al leer las %d entradas del inodo %d.\n ", camino, n_entradas, p_inodo_dir);
						return (-1);
					}
					
					ent[n_entradas+1].inodo = inodo_r;
					strcpy(ent[n_entradas+1].nombre, camino);
				}
				else{
					ent[n_entradas].inodo = inodo_r;
					strcpy(ent[n_entradas].nombre, camino);
				}
				
				/* Introducimos los datos en la entrada */
				
				
				printf(" - ent[%d].inodo = %d\n", n_entradas, ent[n_entradas].inodo);
				printf(" - ent[%d].nombre = %s   // camino = %s\n", n_entradas, ent[n_entradas].nombre, camino);
				
				/* Escribimos la entrada */
				if (mi_write_f(p_inodo_dir, &ent, 0, ((n_entradas+1)*sizeof(struct entrada))) < 0) {
					printf("ERROR (directorios.c -> mi_creat(%s)): Error al escribir la entrada nueva.\n", camino);
					return (-1);
				}
				/* PROVISIONAL */
				else {
					printf(" - Entrada escrita con: p_inodo_dir = %d, y tamaño = %d\n", p_inodo_dir, (n_entradas+1)*sizeof(struct entrada));
					
					struct entrada e[1];
					mi_read_f(p_inodo_dir, &e, 0, ((n_entradas+1)*sizeof(struct entrada)));
					printf(" - Comprobación entrada: nombre = %s, inodo = %d\n", e[0].nombre, e[0].inodo);
					
					printf("FICHERO/DIRECTORIO %s ESCRITO CON ÉXITO !!!\n", camino);
				}
			}
			
			/* Existe la entrada, no podemos crearla */
			else {
				printf("mi_creat: no se puede crear la entrada <<%s>>: El fichero ya existe\n", inicial);
				printf("DEBUG: camino = %s y mierda = %d\n", camino, mierda);
				return (-1);
			}
		}
	}
	
	return (1); /* Ejecución terminada con éxito */
}







