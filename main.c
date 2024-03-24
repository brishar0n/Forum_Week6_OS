#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

// Mutex and condition variables to protect the shared buffer
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_not_empty = PTHREAD_COND_INITIALIZER;

// Buffer for interleaving outputs of the two producer functions
int buffer[BUFFER_SIZE];

int buffer_count = 0;

// Function prototypes
void *producer1(void *ptr);
void *producer2(void *ptr);

int main()
{
    // Initialize random number generator
    srand((unsigned)time(NULL));

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, producer1, NULL);
    pthread_create(&thread2, NULL, producer2, NULL);

    // Wait for both threads to complete
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&buffer_not_full);
    pthread_cond_destroy(&buffer_not_empty);

    return 0;
}

void *producer1(void *ptr)
{
    // Local variable to keep track of the current number being generated
    int num = 0;

    // Producer 1 generates numbers between 1 and 10
    while (1) {
        pthread_mutex_lock(&buffer_mutex);

        // Wait if the buffer is full
        while (buffer_count == BUFFER_SIZE) {
            pthread_cond_wait(&buffer_not_full, &buffer_mutex);
        }

        // Generate a random number and add it to the buffer
        num = 1 + rand() % 10;
        buffer[buffer_count++] = num;

        // Signal that the buffer is not empty
        pthread_cond_signal(&buffer_not_empty);

        // Release the mutex
        pthread_mutex_unlock(&buffer_mutex);

        printf("Producer 1 generated number: %d\n", num);

        // Sleep for a random time between 1 and 5 seconds
        sleep(1 + rand() % 5);
    }
    return NULL;
}

void *producer2(void *ptr)
{
    // Local variable to keep track of the current number being generated
    int num = 0;

    // Producer 2 generates numbers between 10 and 20
    while (1) {
        // Acquire the mutex
        pthread_mutex_lock(&buffer_mutex);

        // Wait if the buffer is full
        while (buffer_count == BUFFER_SIZE) {
            pthread_cond_wait(&buffer_not_full, &buffer_mutex);
        }

        // Generate a random number and add it to the buffer
        num = 10 + rand() % 11;
        buffer[buffer_count++] = num;

        // Signal that the buffer is not empty
        pthread_cond_signal(&buffer_not_empty);

        // Release the mutex
        pthread_mutex_unlock(&buffer_mutex);

        printf("Producer 2 generated number: %d\n", num);

        // Sleep for a random time between 1 and 5 seconds
        sleep(1 + rand() % 5);
    }
    return NULL;
}
