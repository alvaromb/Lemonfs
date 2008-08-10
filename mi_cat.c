#include "bloques.h"
#include "ficheros_basico.h"
#include "ficheros.h"
#include "directorios.h"

#include <stdlib.h>

int main(int argc, char **argv) {
	
	if (argc == 3) {
		
		mount(argv[1]);
		
		struct STAT estado;
		if (mi_stat(argv[2], &estado) < 0) {
			printf("ERROR (mi_cat.c): Fallo al leer el estado de %s.\n", argv[2]);
			return (-1);
		}
		
		char buffer[TB];
		
		int i;
		for (i = 0; (i*TB) < estado.t_bytes; i++) {
			if (mi_read(argv[2], buffer, (i*TB), TB) < 0) {
				printf("ERROR (mi_cat.c): Error al leer con mi_read(%s, buffer, %d, %d).\n", argv[2], (i*TB), TB);
				return (-1);
			}
			
			/* Imprimimos por pantalla */
			printf("%s", buffer);
		}
		
		unmount(argv[1]);
	
	}
	else {
		printf("Error en los parámetros de mi_rm\n");
	}
	
	return (0);
}
