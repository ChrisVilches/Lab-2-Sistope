#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "grupohilo.h"
#include "lib.h"



struct argumento{

	grupohilo* ptr_grupohilo;
	int id_hilo;

};


void inicializar_grupohilo(grupohilo* grupohilo, char* nombre_archivo, int threads_por_equipo, int id_grupo){

	grupohilo->num_threads = threads_por_equipo; 
    grupohilo->hilos = (pthread_t*) malloc(sizeof(pthread_t) * grupohilo->num_threads);
    grupohilo->tiempo_hebra = (double*) malloc(sizeof(double) * grupohilo->num_threads);
  
    grupohilo->id_grupo = id_grupo;

    leer_listas(grupohilo, nombre_archivo);

    inicializar_monitor(&grupohilo->monitor, grupohilo->num_threads, grupohilo->cuantas_listas);

}

void destruir_grupohilo(grupohilo* grupohilo){

	int i;
	free(grupohilo->hilos);
	free(grupohilo->tiempo_hebra);

	for(i=0; i<grupohilo->cuantas_listas; i++){
		free(grupohilo->conjunto_listas[i].num);
	}

	free(grupohilo->conjunto_listas);

}




// Hebra que se dedica a cooperar con su grupo, a intersectar las listas
void* hebra_intersecta(void* arg){

	int i;
	FILE* fp_resultado;

	// Datos para calcular el tiempo
	clock_t begin, end;
	double time_spent;	

	// Se pueden sacar los argumentos de la estructura arg1
	struct argumento arg1 = *((struct argumento*) arg);

	// Extraer argumentos
	int id_hilo = arg1.id_hilo;
	grupohilo* grupohilo = arg1.ptr_grupohilo;
	monitor* monitor = &grupohilo->monitor;

	// Las listas S y K
	lista* S;
	lista* K;

	// Rangos "desde" y "hasta" para calcular cual sera la sublista K de esta hebra
	int desde;
	int hasta;
	int k_dividido_p;
	

	// Comenzar a contar el tiempo
	begin = clock();
	
	// Obtener la lista inicial, la mas corta
	S = &grupohilo->conjunto_listas[0];
	

	while(quedan_listas(monitor) && comprobar_interseccion_no_vacia(monitor))
	{

		K = &grupohilo->conjunto_listas[monitor->lista_actual];


		k_dividido_p = K->tamano/grupohilo->num_threads;

		// Si la division no es perfecta, significa que se aproxima al numero siguiente
		// Esto es el equivalente a obtener [K/P]
		if(K->tamano % grupohilo->num_threads != 0){
			k_dividido_p++;
		}

		// Calcular rangos
		desde = (id_hilo * k_dividido_p);
		hasta = desde + k_dividido_p - 1;

		// Normalizar los rangos.
		// Si "desde" esta dentro de la lista K, pero "hasta" esta fuera, entonces truncar "hasta"
		if(desde < K->tamano && !(hasta < K->tamano)){
			hasta = K->tamano-1;
		} 

		// Si el rango esta dentro de la lista K, entonces procesar
		// Si el rango esta fuera, entonces no hacer nada
		// (Esto pasa cuando hay por ejemplo mas hebras que elementos,
		// como la division debe aproximarse, ocurre que algunas hebras no tendra
		// que trabajar)
		if(desde < K->tamano && hasta < K->tamano){

			// Ordenar la sublista k
			quicksort_lista_limites(K, desde, hasta);

			// Para todo elemento de S
			for(i=0; i<S->tamano; i++){				

				// Buscar si existe S[i] en la lista K
				if(existe_elemento_en_busquedabinaria(S->num[i], K, desde, hasta)){


					// Si esta, entonces agregarlo a la lista S'

					agregar_elemento_sprima(monitor, S->num[i]);
				}
			}
		}



		monitor_termine_de_procesar_una_sublista_k(monitor, S, id_hilo);

	}

	// Detener la cuenta del tiempo
	end = clock();


	// Escribir el resultado en una lista (solo una hebra de un grupo lo puede hacer)
	if(id_hilo == 0 && grupohilo->id_grupo == 0){

		fp_resultado = fopen("resultado.temp", "w");

		for(i=0; i<S->tamano; i++){
			fprintf(fp_resultado, "%d ", S->num[i]);
		}

		//free(S->num);

		fclose(fp_resultado);		
	}

	// Obtener cuanto tiempo tardo
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	// Guardar el tiempo en el arreglo global
	grupohilo->tiempo_hebra[id_hilo] = time_spent;


}


void intersectar_listas(grupohilo* grupohilo, int* mejor_hebra, double* promedio_tiempos, double* mejor_tiempo_hebra){


	int i;
	int min;
	double suma;


	// Crear las ID para cada hilo y ademas guardar el puntero al grupo
	struct argumento* argumento = (struct argumento*) malloc(sizeof(struct argumento) * grupohilo->num_threads);

	// Ordenar listas de tamano menor a tamano mayor (no ordena los elementos de las listas,
	// si no que solamente la posicion de las listas en el arreglo, usando el tamano de la
	// lista como criterio)
	quicksort_arreglo_listas(grupohilo->conjunto_listas, grupohilo->cuantas_listas);

	// Crear altiro la lista S'
	monitor_crear_lista_s_prima(&grupohilo->monitor, grupohilo->conjunto_listas[0].tamano);


	// Enumerar cada hilo
	for(i=0; i<grupohilo->num_threads; i++){
		argumento[i].id_hilo = i;
		argumento[i].ptr_grupohilo = grupohilo;
	}

	// Crear hilos
	for(i=0; i<grupohilo->num_threads; i++){
		pthread_create(&grupohilo->hilos[i], NULL, hebra_intersecta, &argumento[i]);

	}

	// Juntar hilos
	for(i=0; i<grupohilo->num_threads; i++){
		pthread_join(grupohilo->hilos[i], NULL);
	}

	// Obtener cual fue la mejor hebra
	min = 0;
	*mejor_tiempo_hebra = grupohilo->tiempo_hebra[0];

	for(i=1; i<grupohilo->num_threads; i++){
		if(grupohilo->tiempo_hebra[i] < grupohilo->tiempo_hebra[min]){
			min = i;			
		}
	} 

	


	// Obtener promedio
	suma = 0;
	for(i=0; i<grupohilo->num_threads; i++){
		suma += grupohilo->tiempo_hebra[i];
	} 

	*promedio_tiempos = suma/grupohilo->num_threads;
	*mejor_hebra = min;
	*mejor_tiempo_hebra = grupohilo->tiempo_hebra[min];


	free(argumento);


}


// Crea las listas
void leer_listas(grupohilo* grupohilo, char* nombre_archivo){

	FILE* fp;
	char line[4096];
	char line2[4096];
	int longitud_lista;
	char* token;
	int cantidad_listas = 0;
	int i = 0;
	int j;

	fp = fopen(nombre_archivo, "r");

	// Validar el archivo
	if(fp == NULL){
		printf("Archivo no encontrado: %s\n", nombre_archivo);
		abort();
	}

	// Contar la cantidad de lineas que tiene el archivo
	while (fgets(line, sizeof(line), fp) != NULL){
		cantidad_listas++;
	}
	
	// Crear el conjunto de listas
	grupohilo->conjunto_listas = (lista*) malloc(sizeof(lista) * cantidad_listas);
	grupohilo->cuantas_listas = cantidad_listas;

	// Volver al inicio del archivo
	rewind(fp);

	// Crear las listas
	while (fgets(line, sizeof(line), fp) != NULL){

		j=1;

		// Copiar la linea
		strcpy(line2, line);

		// Contar cuantos numeros tiene la lista
		longitud_lista = 0;
		token = strtok(line, " ");
		while(token != NULL && token[0] >= '0' && token[0] <= '9') {			
			token = strtok(NULL, " ");
			longitud_lista++;
		}

		// Crear una nueva lista del tamano leido
		lista nueva;
		nueva.tamano = longitud_lista;
		nueva.num = (int*) malloc(sizeof(int) * longitud_lista);

		// Cortar nuevamente la string, para poder agregar los numeros
		token = strtok(line2, " ");
		nueva.num[0] = atoi(token);

		while(token != NULL) {	
			token = strtok(NULL, " ");	
			if(token != NULL && token[0] >= '0' && token[0] <= '9'){
				nueva.num[j] = atoi(token);
			} else {
				break;
			}	

			j++;
		}

		// Guardar esta lista en la posicion adecuada
		// en el conjunto de listas
		grupohilo->conjunto_listas[i] = nueva;
		i++;
	}
	fclose(fp);	
}
