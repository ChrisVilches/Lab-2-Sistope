# Lab 2 Sistemas Operativos (2-2015)

Creado por Felipe Vilches Cespedes.
RUT 18.015.608-9

Compilacion con 
```
make
```
Luego entrar a la carpeta ```bin``` y ejecutar
```
./competencia -i <nombre archivo> -g <cantidad grupos> -h <cantidad hebras por grupo>
```
Por ejemplo
```
./competencia -i a.txt -g 2 -h 2
```

##Importante

1. Este algoritmo genera una interseccion de varias listas, pero la lista resultante no necesariamente es de menor longitud que las originales. En algunos casos podria ser mas larga, esto es gracias a que asi funciona el algoritmo (entregado en el enunciado), y no porque haya un problema con el codigo.
2. Genera comportamiento indefinido si el archivo de listas tiene lineas extra en blanco.
3. Se han incluido archivos de prueba, ademas de capturas de pantalla que demuestran la ejecucion del programa.
4. Ademas, se incluye un diagrama que muestra la estructura de la solucion.

## Archivos

1. El archivo ```monitor.c``` contiene el codigo del monitor.
2. El archivo ```competencia.c``` contiene el main, y la creacion de los hilos para cada equipo.
3. El archivo ```grupohilo.c``` contiene el codigo para intersectar listas, y creacion y ejecucion de los hilos.
4. El archivo ```lib.c``` contiene varias funciones como ```getopt```, algoritmos de ordenamiento, entre otras cosas.
5. El archivo ```lista.h``` contiene la estructura usada para las listas.


## Caracteristicas tecnicas

1. Para hacer la lectura de los archivos con listas, se tuvo que hacer con exclusion mutua (usando ```pthread_mutex_t```), ya que al tener varios hilos leyendo el archivo al mismo tiempo, se producian errores impredecibles, y las listas eran leidas de manera erronea.
2. Las listas son arreglos. (no se pudo usar listas enlazadas, ya que no permite hacer busquedas binarias)
3. La lista S' es un arreglo hecho con malloc, pero hace realloc (dobla su dimension) cada vez que detecta que le falta memoria. Esto se debe a que la longitud de S' no se puede predecir (asumiendo el peor caso, es un desperdicio de memoria enorme), ya que a veces las intersecciones generan listas S' mas grandes que las originales.
4. Algunos de los tipos y funciones usadas:
	* ```pthread_mutex_t``` 
	* ```pthread_cond_broadcast``` 
	* ```pthread_cond_wait```
	* ```pthread_mutex_lock```
	* ```pthread_mutex_unlock```
	* ```pthread_t```
	* Algoritmos como ```quicksort```, ```insertion sort```, ```busqueda binaria```, entre otros.