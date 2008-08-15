#include "bloques.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "directorios.h"

#include <stdlib.h>

int main(int argc, char **argv) {
	
	if (argc == 3) {
		
		mount(argv[1]);
		
		char buffer[TB*60]; /* Tamaño aleatorio */
		memset(buffer, '\0', (TB*60));
		
		int entradas = mi_dir(argv[2], buffer);
		if (entradas < 0) {
			printf("ERROR (mi_ls.c): Fallo al ejecutar mi_dir(%s, buffer).\n", argv[2]);
			return (-1);
		}
		
		printf("%s\n", buffer);
		
		printf("\n\n Total listado: %d\n\n", entradas);
	
		unmount(argv[1]);
	
	}
	else {
		printf("Error en los parámetros de mi_ls\n");
	}
	
	return (0);
}
