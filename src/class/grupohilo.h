#ifndef GRUPOHILO_H_INCLUDED
#define GRUPOHILO_H_INCLUDED
#include "listas.h"
#include "monitor.h"

class grupohilo{

	// Numero de hilos por grupo
	int num_threads;

	// Cada grupo lee el archivo de texto, y mantiene su propia copia de las listas
	listas conjunto_listas;

	// El monitor que usara el equipo
	monitor mon;

public:

	grupohilo(char* nombre_archivo, int threads_por_equipo);
	~grupohilo();


};

#endif 