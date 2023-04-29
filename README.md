# Laboratorio 4
# Instrucciones de compilación y ejecución de archivos en C
Este documento proporciona instrucciones para compilar y ejecutar tres archivos en C: `riemann.c` , `mpi_trapIO.c` y `mpi_trap4_do2.c`

## Requisitos previos

Antes de continuar con las instrucciones de compilación y ejecución, es necesario asegurarse de que se cumplan los siguientes requisitos previos:

- Tener instalado un compilador de C (por ejemplo, gcc)
- Tener instalado MPI (Message Passing Interface)

## Compilación

A continuación se muestran las instrucciones para compilar cada uno de los archivos en C:

## mpi_trapIO.c, riemann.c y mpi_trap4_do2.c
Para compilar este archivo, es necesario utilizar el compilador de MPI. Se puede utilizar el siguiente comando para compilar el archivo:

```sh
mpicc mpi_trapIO.c -o mpi_trapIO -lm
mpicc -o mpi_trap4_do2 mpi_trap4_do2.c -lm
gcc riemann.c -o riemann_seq -fopenmp
```

## Ejecución

A continuación se muestran las instrucciones para ejecutar cada uno de los archivos compilados anteriormente:

## riemann.c

```sh
./riemann_seq
```

Para ejecutar este archivo, es necesario utilizar el comando "mpirun" seguido del número de procesos que se desean ejecutar. El siguiente comando se puede utilizar para ejecutar el archivo:

```sh
mpirun -n 4 ./mpi_trapIO
mpirun -np 4 ./mpi_trap4_do2
```