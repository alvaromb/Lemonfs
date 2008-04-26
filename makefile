#MAKEFILE DE ASO, v1.0
#USO DE VARIABLES:
#   CC = compilador
#   OBJECTS = ficheros adicionales

#VARIABLES:
CC = gcc
OBJECTS = bloques.o 

#ficheros_basico.o ficheros.o

#FUNCIÓN PRINCIPAL:
mi_mkfs	:	mi_mkfs.o  $(OBJECTS)
			$(CC) -o mi_mkfs mi_mkfs.o $(OBJECTS)
            

#OTRAS FUNCIONES:
mi_mkfs.o	:	mi_mkfs.c
				$(CC) -c mi_mkfs.c
                
bloques.o	:	bloques.c bloques.h
				$(CC) -c bloques.c
				
ficheros_basico.o	:	ficheros_basico.c ficheros_basico.h
						$(CC) -c ficheros_basico.c
						
ficheros.o	:	ficheros.c ficheros.h
				$(CC) -c ficheros.c
				

clean: 
	rm mi_mkfs \
		domecillo mi_mkfs.o bloques.o ficheros_basico.o ficheros.o


