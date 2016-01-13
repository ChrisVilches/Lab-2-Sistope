# Lab 2 Sistope

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
3. El archivo ```monitor.c``` contiene el codigo del monitor.
4. El archivo ```competencia.c``` contiene el main, y la creacion de los hilos para cada equipo.
5. El archivo ```grupohilo.c``` contiene el codigo para intersectar listas, y creacion y ejecucion de los hilos.
6. Se han incluido archivos de prueba, ademas de capturas de pantalla que demuestran la ejecucion del programa.

## Caracteristicas tecnicas

1. Para hacer la lectura de los archivos con listas, se tuvo que hacer con exclusion mutua (usando ```pthread_mutex_t```), ya que al tener varios hilos leyendo el archivo al mismo tiempo, se producian errores impredecibles, y las listas eran leidas de manera erronea.
2. Las listas son arreglos. (no se pudo usar listas enlazadas, ya que no permite hacer busquedas binarias)
3. La lista S' es un arreglo hecho con malloc, pero hace realloc (dobla su dimension) cada vez que detecta que le falta memoria. Esto se debe a que la longitud de S' no se puede predecir (asumiendo el peor caso, es un desperdicio de memoria enorme), ya que a veces las intersecciones generan listas S' mas grandes que las originales.
4. Algunas de los tipos y funciones usadas:
	4.1 ```pthread_mutex_t``` 
	4.2 ```pthread_cond_broadcast``` 
	4.3 ```pthread_cond_wait```
	4.4 ```pthread_mutex_lock```
	4.5 ```pthread_mutex_unlock```