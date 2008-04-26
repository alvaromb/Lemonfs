#include "bloques.h"

static int fs = 0;

int bmount(const char *camino) {

	fs = open(camino, O_RDWR | O_CREAT, S_IRWXU);
	
	if (fs < 0) {
		printf("ERROR (bloques.c -> bmount(%s)): Error al ejecutar open, fs = %d.", camino, fs);
		return (-1);
	}
	else if ((DEBUG > 0) || (DEBUG_BLOQUES > 0)) {
		printf("DEBUG (bloques.c -> bmount(%s)): Open realizado con exito, fs = %d.", camino, fs);
	}
	
	return fs;
}


int bumount() {

	int fclose = close(fs);
	
	if (fclose < 0) {
		printf("ERROR (bloques.c -> bumount()): Error al cerrar, fclose = %d.", fclose);
		return (-1);
	}
	else if ((DEBUG > 0) || (DEBUG_BLOQUES > 0)) {
		printf("DEBUG (bloques.c -> bumount()): Fichero cerrado con exito, fclose = %d.", fclose);
	}
	
	return (1);
}


int bwrite(unsigned int bloque, const void *buf) {
	
	int seek = lseek(fs, bloque*TB, SEEK_SET);
	
	if (seek < 0) {
		printf("ERROR (bloques.c -> bwrite(%d, buf)): Error al posicionarse, seek = %d.", bloque, seek);
		return (-1);
	}
	
	if (write(fs, buf, TB) < 0) {
		printf("ERROR (bloques.c -> bwrite(%d, buf)): Error al escribir, fs = %d.", bloque, fs);
		return (-1);
	}
	else if ((DEBUG > 0) || (DEBUG_BLOQUES > 0)) {
		printf("DEBUG (bloques.c -> bwrite(%d, buf)): Escritura completada con exito.", bloque);
	}
	
	return (1);
}


int bread(unsigned int bloque, void *buf) {

	int seek = lseek(fs, bloque*TB, SEEK_SET);
	
	if (seek < 0) {
		printf("ERROR (bloques.c -> bread(%d, buf)): Error al posicionarse, seek = %d.", bloque, seek);
		return (-1);
	}
	
	if (read(fs, buf, TB) < 0) {
		printf("ERROR (bloques.c -> bread(%d, buf)): Error al leer, fs = %d.", bloque, fs);
		return (-1);
	}
	else if ((DEBUG > 0) || (DEBUG_BLOQUES > 0)) {
		printf("DEBUG (bloques.c -> bread(%d, buf)): Lectura completada con exito.", bloque);
	}
	
	return (1);
}
