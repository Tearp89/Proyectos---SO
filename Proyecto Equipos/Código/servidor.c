#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <assert.h>
#define NOMBRE_SEMAFORO "semaforo"
#define NOMBRE_SEMAFORO2 "semaforo2"
#define NOMBRE_SEMAFORO3 "semaforo3"
#define NOMBRE_SEMAFORO4 "semaforo4"
#define PERMISOS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define FLAGS (O_CREAT | O_EXCL)

int main(int argc, char *argv[]){
    const char *nombre = "memoria_compartida";
    const int tam = 4096;
    char mensaje[200];
    void *zona_memoria;

    sem_unlink(NOMBRE_SEMAFORO);
    sem_unlink(NOMBRE_SEMAFORO2);
    sem_unlink(NOMBRE_SEMAFORO3);
    sem_unlink(NOMBRE_SEMAFORO4);
    sem_t *semaforo = sem_open(NOMBRE_SEMAFORO, FLAGS, PERMISOS, 0);
    assert(semaforo != SEM_FAILED);
    sem_t *semaforo2 = sem_open(NOMBRE_SEMAFORO2, FLAGS, PERMISOS, 0);
    assert(semaforo2 != SEM_FAILED);
    sem_t *semaforo3 = sem_open(NOMBRE_SEMAFORO3, FLAGS, PERMISOS, 0);
    assert(semaforo3 != SEM_FAILED);
    sem_t *semaforo4 = sem_open(NOMBRE_SEMAFORO4, FLAGS, PERMISOS, 0);
    assert(semaforo4 != SEM_FAILED);
    int shm_fd = shm_open(nombre, O_CREAT | O_RDWR, 0600);
    if(shm_fd == -1){
        perror("Error al crear la memoria compartida");
        return -1;
    }
    if(ftruncate(shm_fd, tam) == -1){
        perror("Error al vaciar la memoria");
        return -1;
    }

    zona_memoria = mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(zona_memoria == MAP_FAILED){
        perror("Error al mapear la memoria compartida");
        return -1;
    }

    sem_wait(semaforo);
    //fgets(mensaje, 200, stdin);
    strncpy(mensaje, zona_memoria, 200);
    printf("El mensaje recibido es %s", mensaje);
    printf("Enviando el mensaje al convertidor");
    sprintf(zona_memoria, "%s", mensaje);
    sem_post(semaforo2);

    /*sprintf(zona_memoria, "%s", mensaje);
    sem_post(semaforo);*/

    sem_wait(semaforo3);
    strncpy(mensaje, zona_memoria, 200);
    printf("\nenviando al cliente principal");
    sem_post(semaforo4);
    getc(stdin);

    sem_unlink(NOMBRE_SEMAFORO);
    sem_unlink(NOMBRE_SEMAFORO2);
    sem_unlink(NOMBRE_SEMAFORO3);
    sem_unlink(NOMBRE_SEMAFORO4);

    if(munmap(zona_memoria, tam) == -1){
        perror("Error al desasignar la memoria");
        return -1;
    }

    if(close(shm_fd) == -1){
        perror("Error al cerrar el descriptor de archivo");
    }
    if(shm_unlink(nombre) == -1){
        perror("Error al remover la memoria compartida");
        return -1;
    }

    return 0;
}