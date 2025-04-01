**FreeRTOS-sharedmem.c:**
- code that displays how two FreeRTOS specific tasks communicate (exchange data) with shared memory.
- Since both tasks are part of the same process address space, shared memory is allocated in the global address space of the process and it can be synchronized by binary semaphore (mutex).  

**QNX RTOS (Any Posix compliant OS):**
- two processes with different process address space need to exchange data, both use
- shm_open(): to get handle to shared memory.
- mmap()/unmap(): map the same chunk of memory(hence shared) to their address space.
- sem_wait()/sem_post(): synchronize access to the memory to avoid race conditions.

**posix-sharedmem-writer.c:**
- set the size of the shared memory. 
- waits on semaphore, writes the data and posts the semaphore for the reader to read the data.

**posix-sharedmem-reader.c:**
- waits for the writer to post the semaphore before going ahead to read it.
