#ifndef LISTAS_H_INCLUDED
#define LISTAS_H_INCLUDED

typedef struct {
	int tamano;
	int* num;
} lista;

class listas{
	
public:
	listas(){}
	listas(char* nombre_archivo);
	void mostrarlista(int indice);
	lista* li;
	int cuantas_listas;
};


#endif