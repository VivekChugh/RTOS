#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/stat.h>

#define SHM_NAME "/my_shm"
#define SEM_NAME "/my_semaphore"

typedef struct {
    int data;
} shared_memory_t;

int main() {
    // Open shared memory
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    // Map shared memory
    shared_memory_t *shm_ptr = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    // Open semaphore
    sem_t *sem = sem_open(SEM_NAME, 0);  // Open the existing semaphore
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    // Read data from shared memory
    for (int i = 0; i < 5; ++i) {
        sem_wait(sem);  // Wait (lock)
        
        printf("Process 2: read %d from shared memory\n", shm_ptr->data);
        
        sem_post(sem);  // Signal (unlock)
        
        sleep(1);  // Simulate work
    }

    // Clean up
    sem_close(sem);
    munmap(shm_ptr, sizeof(shared_memory_t));
    close(shm_fd);

    return 0;
}
