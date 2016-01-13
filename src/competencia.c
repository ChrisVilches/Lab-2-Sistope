#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib/lista.h"
#include "lib/grupohilo.h"
#include "lib/lib.h"


/*
*	Variables globales
*/
int cantidad_equipos = 0;
int threads_por_equipo = 0;
char* nombre_archivo;
double* tiempos_equipos;	// Sirve para que cada equipo guarde sus tiempos (para que luego el main los vea todos)
int* mejor_hebra;			// Cada equipo almacenara en este arreglo (posicion del equipo), la mejor hebra de su equipo
double* mejor_hebra_tiempo;	// Almacena el tiempo de la mejor hebra
double* promedios_tiempos;	// Cada equipo guarda aca el promedio de los tiempos de todas sus hebras


/*
*	Este procedimiento corresponde a lo que hace un equipo
*	Cada equipo se representa por un hilo
*/
void* ejecutar_hilo_equipo(void* arg){

	// Obtener la ID del equipo, pasada desde el argumento
	int id_equipo = *((int*) arg);
	clock_t begin, end;
	double time_spent;
	int id_mejor_hebra_intersecta;
	double mejor_tiempo_hebra;
	double promedio_tiempos;
	

	// Se crea el grupo
	grupohilo equipo;

	inicializar_grupohilo(&equipo, nombre_archivo, threads_por_equipo, id_equipo);


	// Comenzar a contar el tiempo
	begin = clock();

	// Comenzar a intersectar las listas
	// Dado que la funcion intersectar_listas provee los mecanismos de Join, y crear hebras
	// no es necesario hacerlo aca

	intersectar_listas(&equipo, &id_mejor_hebra_intersecta, &promedio_tiempos, &mejor_tiempo_hebra);		

	// Detener la cuenta del tiempo
	end = clock();

	// Liberar memoria del grupo
	destruir_grupohilo(&equipo);

	// Obtener cuanto tiempo tardo
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Guardar el tiempo en el arreglo global
	tiempos_equipos[id_equipo] = time_spent;

	// Guardar en el arreglo cual fue la mejor hebra de este equipo
	mejor_hebra[id_equipo] = id_mejor_hebra_intersecta;

	// ... y guardar su tiempo
	mejor_hebra_tiempo[id_equipo] = mejor_tiempo_hebra;

	// Guardar promedio
	promedios_tiempos[id_equipo] = promedio_tiempos;

}

int main(int argc, char** argv){
	
	char option;
	int i;
	pthread_t* grupos_hilos;
	int* id_equipos;
	int hebra_mas_eficiente;
	int grupo_mas_eficiente;
	int tres_mejores[3];	
	FILE* fp_resultado;
	char leido;

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
		printf("Argumentos invalidos\n");
		exit(1);
	}

	// Crear el arreglo para que cada equipo almacene sus tiempos
	tiempos_equipos = (double*) malloc(sizeof(double) * cantidad_equipos);

	// Arreglo donde cada equipo almacenara su mejor hebra
	mejor_hebra = (int*) malloc(sizeof(int) * cantidad_equipos);

	// Aca van los tiempos de las mejores hebras de cada grupo
	mejor_hebra_tiempo = (double*) malloc(sizeof(double) * cantidad_equipos);

	// Promedios de las hebras de cada grupo (tiempos)
	promedios_tiempos = (double*) malloc(sizeof(double) * cantidad_equipos);

	// Crear equipos, cada equipo es un hilo
	grupos_hilos = (pthread_t*) malloc(sizeof(pthread_t) * cantidad_equipos);

	// Crear las IDs de los equipos
	id_equipos = (int*) malloc(sizeof(int) * cantidad_equipos);


	pthread_mutex_init(&leer_archivos_mutex, NULL);

	// Enumerar las ID desde 0 hasta N-1 (cantidad de equipos)
	for(i=0; i<cantidad_equipos; i++){
		id_equipos[i] = i;
	}

	// Crear todos los hilos para cada grupo
	for(i=0; i<cantidad_equipos; i++){
		pthread_create(&grupos_hilos[i], NULL, ejecutar_hilo_equipo, &id_equipos[i]);
	}

	
	
	/*
	*
	*	(En este punto, la competencia ya empezo)
	*
	*/

	// Juntar todos los hilos (cada grupo)
	for(i=0; i<cantidad_equipos; i++){
		pthread_join(grupos_hilos[i], NULL);
	}

	printf("\n\n**********Fin de la competencia*************\n\n");

	// Encontrar los tres mejores lugares
	tres_primeros_lugares(tres_mejores, cantidad_equipos, tiempos_equipos);

	// Mostrar los tres mejores
	for(i=0; i<3; i++){
		if(tres_mejores[i] == -1){
			break;
		}
		printf("Lugar #%d, equipo ID=%d (tiempo %f)\n", i+1, tres_mejores[i], tiempos_equipos[tres_mejores[i]]);
	}


	// Encontrar la hebra mas eficiente
	hebra_mas_eficiente = 0;

	for(i=1; i<cantidad_equipos; i++){
		if(mejor_hebra_tiempo[i] < mejor_hebra_tiempo[hebra_mas_eficiente]){
			hebra_mas_eficiente = i;
		}
	}

	// Encontrar el grupo que en promedio fue mas rapido
	grupo_mas_eficiente = 0;

	for(i=1; i<cantidad_equipos; i++){
		if(promedios_tiempos[i] < promedios_tiempos[grupo_mas_eficiente]){
			grupo_mas_eficiente = i;
		}
	}



	// La hebra mas eficiente
	printf("La hebra mas rapida fue la hebra %d, del equipo %d (tiempo %f)\n", mejor_hebra[hebra_mas_eficiente], hebra_mas_eficiente, mejor_hebra_tiempo[hebra_mas_eficiente]);

	// El grupo que en promedio fue mas rapido
	printf("El grupo mas rapido fue %d. El promedio de tiempo de sus hebras es %f\n", grupo_mas_eficiente, promedios_tiempos[grupo_mas_eficiente]);


	// Leer los resultados (fueron escritos a un archivo)
	fp_resultado = fopen("resultado.temp", "r");
	if(fp_resultado == NULL){

		// No se leyo la lista
		printf("Lista interseccion: []\n");
	} else {
		// Se leyo la lista

		printf("Lista interseccion: [ ");
		while ((leido = getc(fp_resultado)) != EOF)
			printf("%c", leido);

		printf("]\n");
		
		fclose(fp_resultado);

		// Eliminar el archivo temporal
		remove("resultado.temp");
	}
	


	// Liberar memoria

	free(mejor_hebra_tiempo);
	free(promedios_tiempos);
	free(mejor_hebra);
	free(tiempos_equipos);
	free(grupos_hilos);
	free(id_equipos);
	
	return 0;

}