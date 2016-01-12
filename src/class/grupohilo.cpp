#include <iostream>
#include <stdlib.h>
#include "grupohilo.h"
#include "monitor.h"

grupohilo::grupohilo(char* nombre_archivo, int threads_por_equipo) : 
																	num_threads(threads_por_equipo), 
																	conjunto_listas(nombre_archivo){


    std::cout << "Soy un grupo (constructor con argumentos)" << std::endl;

}


grupohilo::~grupohilo(){
	int i;

	std::cout << "Destroy grupohilo" << std::endl;	
	for(i=0; i<conjunto_listas.cuantas_listas; i++){
		
	}
	free(conjunto_listas.li);
}
