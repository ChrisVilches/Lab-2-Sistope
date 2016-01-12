DIR_OUTPUT = ./bin
DIR_SRC = ./src
NOMBRE_MAIN = main
OBJ = $(DIR_OUTPUT)/listas.o $(DIR_OUTPUT)/grupohilo.o

all: dir main cleanobj

dir: 
# Crear el directorio "bin"
	mkdir -p $(DIR_OUTPUT)

main: monitor.o listas.o grupohilo.o
# Compilar el main
	g++ $(OBJ) $(DIR_SRC)/$(NOMBRE_MAIN).cpp -o $(DIR_OUTPUT)/$(NOMBRE_MAIN)

listas.o:
	g++ -c $(DIR_SRC)/class/listas.cpp -o $(DIR_OUTPUT)/listas.o

grupohilo.o:
	g++ -c $(DIR_SRC)/class/grupohilo.cpp -o $(DIR_OUTPUT)/grupohilo.o

monitor.o:
	g++ -c $(DIR_SRC)/class/monitor.cpp -o $(DIR_OUTPUT)/monitor.o

clean:
	rm -rf $(DIR_OUTPUT)/*

cleanobj:
	rm -rf $(DIR_OUTPUT)/*.o