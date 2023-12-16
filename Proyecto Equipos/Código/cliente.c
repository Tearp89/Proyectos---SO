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
#define NOMBRE_SEMAFORO2 "semaforo2"
#define NOMBRE_SEMAFORO3 "semaforo3"
#define PERMISOS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define FLAGS (O_EXCL)

void convertirAMayusculas(char cadena[]) {
    int i = 0;

    while (cadena[i] != '\0') {
        if (cadena[i] >= 'a' && cadena[i] <= 'z') {
            // Convierte letra minúscula a mayúscula
            cadena[i] = cadena[i] - 'a' + 'A';
        }
        i++;
    }
}

int main(int argc, char *argv[]){
    const char *nombre = "memoria_compartida";
    char mensaje[200];
    const int tam = 4096;
    void *zona_memoria;

    sem_t *semaforo = sem_open(NOMBRE_SEMAFORO, FLAGS, PERMISOS, 0);
    assert(semaforo != SEM_FAILED);
    sem_t *semaforo2 = sem_open(NOMBRE_SEMAFORO2, FLAGS, PERMISOS, 0);
    assert(semaforo2 != SEM_FAILED);
    sem_t *semaforo3 = sem_open(NOMBRE_SEMAFORO3, FLAGS, PERMISOS, 0);
    assert(semaforo3 != SEM_FAILED);

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

    sem_wait(semaforo2);
    strncpy(mensaje, zona_memoria, 200);
    convertirAMayusculas(mensaje);
    printf("mensaje recibido, convirtiendo mensaje");

    // Copiar la cadena convertida de vuelta a la memoria compartida
    strncpy(zona_memoria, mensaje, 200);
    
    printf("Regresando el mensaje al cliente");
    sem_post(semaforo3);

    if(munmap(zona_memoria, tam) == -1){
        perror("Error al desasignar la memoria");
        return -1;
    }

    if(close(shm_fd) == -1){
        perror("Error al cerrar el descriptor de archivo");
    }

    return 0;
}
