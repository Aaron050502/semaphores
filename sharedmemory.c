#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define MAX_ELEMENTS 10

#define SHARED_MEMORY_NAME "/memoryThree"
#define SEM_NAME_WRITE "/sem_write"
#define SEM_NAME_READ "sem_read"


typedef struct {
    int value;
} BufferElement;

typedef struct {
    BufferElement buffer[MAX_ELEMENTS];
    int head;
    int tail;
    int count;
} Buffer;


int main(int argc, char *argv[]) {

    int shmfd = shm_open(SHARED_MEMORY_NAME, O_RDWR | O_CREAT, 0600);
    if (shmfd == -1) {
        fprintf(stderr, "Error creatin shared memory");
        exit(1);
    }
    if (ftruncate(shmfd, sizeof(Buffer) < 0)) {
        fprintf(stderr, "Error allocating memory");
    }

    Buffer *buffer;
    buffer = mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (buffer == MAP_FAILED) {
        fprintf(stderr, "Failed the mapping");
    }

    buffer->head = 0;
    buffer->tail = 0;
    buffer->count = 0;

    sem_t *sem_write = sem_open(SEM_NAME_WRITE, O_CREAT, 0600, 1);
    if (sem_write == SEM_FAILED) {
        perror("sem_open (write)");
        return 1;
    } 

    sem_t *sem_read = sem_open(SEM_NAME_READ, O_CREAT, 0600, 1);
    if (sem_read == SEM_FAILED) {
        perror("sem_write (error)");
        return 1;
    }



    if (close (shmfd) == -1) {
        fprintf(stderr, "error clossing fd");
    }

    printf("Shared buffler and semaphore intialized.\n");

    getchar();

    munmap(buffer ,sizeof(Buffer));
    close(shmfd);
    shm_unlink(SHARED_MEMORY_NAME);
    sem_unlink(SEM_NAME_WRITE);
    sem_unlink(SEM_NAME_READ);




    return 0;
}