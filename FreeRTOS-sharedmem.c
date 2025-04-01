#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <stdio.h>

// Shared memory (global variable)
int sharedMemory = 0;

// Binary semaphore for synchronization
SemaphoreHandle_t xSemaphore = NULL;

// Task 1: Write to shared memory
void Task1(void *pvParameters)
{
    int value = 0;

    while(1)
    {
        // Wait for the semaphore to be available
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Write data to shared memory
            sharedMemory = value;
            printf("Task 1 wrote: %d\n", sharedMemory);

            // Update value for the next write
            value++;

            // Release the semaphore so that Task 2 can access the shared memory
            xSemaphoreGive(xSemaphore);
        }

        // Delay to simulate work (in real scenarios this could be some actual processing)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task 2: Read from shared memory
void Task2(void *pvParameters)
{
    while(1)
    {
        // Wait for the semaphore to be available
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Read and process data from shared memory
            int value = sharedMemory;
            printf("Task 2 read: %d\n", value);

            // Release the semaphore so that Task 1 can access the shared memory
            xSemaphoreGive(xSemaphore);
        }

        // Delay to simulate work (in real scenarios this could be some actual processing)
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void)
{
    // Initialize the semaphore with a value of 1 (binary semaphore)
    xSemaphore = xSemaphoreCreateBinary();
    if (xSemaphore != NULL)
    {
        // Give the semaphore initially so that Task 1 can start working
        xSemaphoreGive(xSemaphore);

        // Create Task 1 and Task 2
        xTaskCreate(Task1, "Task1", 1000, NULL, 1, NULL);
        xTaskCreate(Task2, "Task2", 1000, NULL, 1, NULL);

        // Start the FreeRTOS scheduler
        vTaskStartScheduler();
    }
    else
    {
        printf("Semaphore creation failed!\n");
    }

    // If all goes well, the scheduler should be running, and this part of the code will never be reached.
    for (;;);
    return 0;
}
