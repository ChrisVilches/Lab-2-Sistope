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