// As this program uses process threads,
// use "gcc -pthread producer_consumer_problem-seamaphore.c" to compile
// instead of "gcc producer_consumer_problem-seamaphore.c"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>                         // for malloc
#include <semaphore.h>                      // for semaphore
sem_t *empty, *full, *mutex;
unsigned int delay;
// Circular queue is preferred
int buf[1000], buf_begin=-1, buf_end = -1;
void* producer(void* arg1){
    while(1){
        printf("\nEnter an integer to produce:   ");
        int next_produced;
        scanf("%d",&next_produced);
        printf("%d is produced\n",next_produced);
        printf("Waiting for empty buffer\n");
        sem_wait(empty);
        sem_wait(mutex);
        if(buf_begin == buf_end){
            buf_begin = 0;
            buf_end = 1;
            buf[buf_begin] = next_produced;
            printf("%d is added to the buffer\n",next_produced);
        } else{
            buf[buf_end++] = next_produced;
            printf("%d is added to the buffer\n",next_produced);
        }
        sem_post(mutex);
        sem_post(full);
    }
}
void* consumer(void* arg1){
    while(1){
        // consumes every "delay" seconds
        sleep(delay);
        int next_produced;
        sem_wait(full);
        sem_wait(mutex);
        if(buf_begin<buf_end){
            next_produced = buf[buf_begin++];
            printf("\n%d is removed from the buffer\n",next_produced);
        }
        sem_post(mutex);
        sem_post(empty);
        printf("Consumer thread consumed: %d\n",next_produced);
        if(buf_begin == buf_end){
            buf_begin = -1;
            buf_end = -1;
            printf("\n\nAll buffers empty!\n");
        }
    }
}
int main(){
    pthread_t* pid = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_t* cid = (pthread_t*)malloc(sizeof(pthread_t));
    empty = (sem_t*)malloc(sizeof(sem_t));
    full = (sem_t*)malloc(sizeof(sem_t));
    mutex = (sem_t*)malloc(sizeof(sem_t));
    int n;
    printf("Enter the number of buffers: ");
    scanf("%d",&n);
    printf("Enter delay time of the consumer in seconds: ");
    scanf("%d",&delay);
    if(sem_init(empty,0,n) == -1){
        // pshared is 0 - to share semapfore between different threads in the program
		printf("\nCannot initialze semapfore\n");
		exit(1);
    }
    if(sem_init(full,0,0) == -1){
        // pshared is 0 - to share semapfore between different threads in the program
		printf("\nCannot initialze semapfore\n");
		exit(1);
    }
    if(sem_init(mutex,0,1) == -1){
        // pshared is 0 - to share semapfore between different threads in the program
		printf("\nCannot initialze semapfore\n");
		exit(1);
    }
	pthread_create(pid,NULL,producer,NULL);
	pthread_create(cid,NULL,consumer,NULL);
	pthread_join(*pid,NULL);
	pthread_join(*cid,NULL);
    return 0;
}
