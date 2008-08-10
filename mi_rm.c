#include "bloques.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "directorios.h"

#include <stdlib.h>

int main(int argc, char **argv) {
	
	if (argc == 3) {
		
		mount(argv[1]);
		
		if (mi_unlink(argv[2]) < 0) {
			printf("ERROR (mi_rm.c): Fallo al ejecutar mi_unlink(%s).\n", argv[2]);
			return (-1);
		}
		
		unmount(argv[1]);
	
	}
	else {
		printf("Error en los parámetros de mi_rm\n");
	}
	
	return (0);
}
