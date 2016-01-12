#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "class/listas.h"
#include "class/grupohilo.h"

using namespace std;

int main(int argc, char** argv){

	int cantidad_equipos = 0;
	int threads_por_equipo = 0;
	char* nombre_archivo;
	char option;
	int i;

	int flagarg1 = 0;
	int flagarg2 = 0;
	int flagarg3 = 0;

	// Obtener opciones

	while ((option = getopt(argc, argv, "g:h:i:")) != -1)
	    switch (option)
	    {
		    case 'g':
			    cantidad_equipos = atoi(optarg);
			    flagarg1++;
			    break;
		    case 'h':
			    threads_por_equipo = atoi(optarg);
			    flagarg2++;
			    break;
		    case 'i':
			    nombre_archivo = optarg;
			    flagarg3++;
			    break;		    
		    default:
		    	abort();
	    }

	// Validar opciones
	if(flagarg1 == 0 || flagarg2 == 0 || flagarg3 == 0){
		cout << "Argumentos invalidos" << endl;
		exit(1);
	}

	// Crear equipos

	grupohilo** grupos = new grupohilo*[cantidad_equipos];

	for(i=0; i<cantidad_equipos; i++){
		grupos[i] = new grupohilo(nombre_archivo, threads_por_equipo);
	}

	// Ahora todos los equipos tienen sus listas



	// Liberar memoria
	for(i=0; i<cantidad_equipos; i++){
		delete grupos[i];
	}

	
	return 0;

}