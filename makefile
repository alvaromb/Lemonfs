#MAKEFILE DE ASO, v1.0
#USO DE VARIABLES:
#   CC = compilador
#   OBJECTS = ficheros adicionales

#VARIABLES:
CC = gcc
OBJECTS = bloques.o ficheros_basico.o ficheros.o directorios.o semaforos.o

#FUNCIÓN PRINCIPAL:
total	:	mi_mkfs mi_cat mi_ls mi_rm simulacion prueba

mi_mkfs	:	mi_mkfs.c  $(OBJECTS)
			$(CC) -o mi_mkfs mi_mkfs.c $(OBJECTS)
			
mi_cat	:	mi_cat.c $(OBJECTS)
			$(CC) -o mi_cat mi_cat.c $(OBJECTS)
			
mi_ls	: 	mi_ls.c $(OBJECTS)
			$(CC) -o mi_ls mi_ls.c $(OBJECTS)
			
mi_rm	:	mi_rm.c $(OBJECTS)
			$(CC) -o mi_rm mi_rm.c $(OBJECTS)
			
simulacion	:	simulacion.c $(OBJECTS)
				$(CC) -o simulacion simulacion.c $(OBJECTS)
				
prueba	:	prueba.c $(OBJECTS)
			$(CC) -o prueba prueba.c $(OBJECTS)
            

#FICHEROS ADICIONALES:                
bloques	:	bloques.c bloques.h
			$(CC) -c bloques.c
				
ficheros_basico	:	ficheros_basico.c ficheros_basico.h
					$(CC) -c ficheros_basico.c
						
ficheros	:	ficheros.c ficheros.h
				$(CC) -c ficheros.c
				
directorios	:	directorios.c directorios.h
				$(CC) -c directorios.c
				
semaforos	:	semaforos.c semaforos.h
				$(CC) -c semaforos.c
				

clean: 
	rm mi_mkfs mi_cat mi_ls mi_rm simulacion prueba \
		disco.imagen $(OBJECTS)


