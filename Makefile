DIR_OUTPUT = ./bin
DIR_SRC = ./src
NOMBRE_MAIN = main
LIB = lib
OBJ = $(DIR_OUTPUT)/listas.o $(DIR_OUTPUT)/grupohilo.o $(DIR_OUTPUT)/lib.o

all: dir main cleanobj

dir: 
# Crear el directorio "bin"
	mkdir -p $(DIR_OUTPUT)

main: lib.o monitor.o listas.o grupohilo.o
# Compilar el main
	gcc $(OBJ) $(DIR_SRC)/$(NOMBRE_MAIN).c -o $(DIR_OUTPUT)/$(NOMBRE_MAIN) -pthread

listas.o:
	gcc -c $(DIR_SRC)/$(LIB)/listas.c -o $(DIR_OUTPUT)/listas.o

grupohilo.o:
	gcc -c $(DIR_SRC)/$(LIB)/grupohilo.c -o $(DIR_OUTPUT)/grupohilo.o

monitor.o:
	gcc -c $(DIR_SRC)/$(LIB)/monitor.c -o $(DIR_OUTPUT)/monitor.o

lib.o:
	gcc -c $(DIR_SRC)/$(LIB)/lib.c -o $(DIR_OUTPUT)/lib.o

clean:
	rm -rf $(DIR_OUTPUT)/*

cleanobj:
	rm -rf $(DIR_OUTPUT)/*.o