//marla aguilar ariana corona
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>

#define NOMBRE_SEMAFORO "semaforo"
#define NOMBRE_SEMAFORO4 "semaforo4"
#define PERMISOS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define FLAGS (O_EXCL)

int main(int argc, char *argv[]){
    const char *nombre = "memoria_compartida";
    char mensaje[200];
    const int tam = 4096;
    void *zona_memoria;

    sem_t *semaforo = sem_open(NOMBRE_SEMAFORO, FLAGS, PERMISOS, 0);
    assert(semaforo != SEM_FAILED);
    sem_t *semaforo4 = sem_open(NOMBRE_SEMAFORO4, FLAGS, PERMISOS, 0);
    assert(semaforo4 != SEM_FAILED);

    int shm_fd = shm_open(nombre, O_RDWR, 0600);
    if(shm_fd == -1){
        perror("Error al abrir la memoria compartida");
        return -1;
    }

    zona_memoria  = mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(zona_memoria == MAP_FAILED) {
        perror("Error al mapear la memoria compartida");
        return -1;
    }
    //lee el mensaje ingresado por el usuario y le da paso al servidor
    printf("Mensaje a enviar: ");
    fgets(mensaje, 200, stdin);

    sprintf(zona_memoria, "%s", mensaje);
    sem_post(semaforo);


    sem_wait(semaforo4);
    strncpy(mensaje, zona_memoria, 200);
    printf("El mensaje recibido es: %s", mensaje);

    if(munmap(zona_memoria, tam) == -1){
        perror("Error al desasignar la memoria");
        return -1;
    }

    if(close(shm_fd) == -1){
        perror("Error al cerrar el descriptor de archivo");
    }

    return 0;
}
