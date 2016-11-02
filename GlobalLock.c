#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

typedef struct node
{
	struct node *next;
} node;

typedef struct queue
{
	node *head;
	node *tail;

	pthread_mutex_t GlobalLock;
} queue;

void Queue_Init(struct queue *q);
void Queue_Enqueue(struct queue *q);
int Queue_Dequeue( struct queue *q);
void* Thread_Enqueue(void *q);
void* Thread_Dequeue(void *q);


int main(){
	
	struct queue MainQueue;
	Queue_Init(&MainQueue);
	
	int i = 0;
	for(i = 0; i< 100; i++){
		Queue_Enqueue(&MainQueue);
	}
	
	pthread_t enQ;
	pthread_t deQ;
	
	pthread_create(&enQ, NULL, Thread_Enqueue, (void*) &MainQueue);	
	pthread_create(&deQ, NULL, Thread_Dequeue, (void*) &MainQueue);
	
	pthread_join(enQ, NULL);
	pthread_join(deQ, NULL);
	
	isEmpty(&MainQueue);	
}

void Queue_Init(struct queue *q){
	struct node *tmp = malloc(sizeof(node));
	tmp->next = NULL;
	q->head = tmp;
	q->tail = tmp;
	pthread_mutex_init(&q->GlobalLock, NULL);
}

void isEmpty(struct queue *q){
	struct node *tmp = q->head;
	struct node *newHead = tmp->next;
	if(newHead == NULL){
		printf("Queue is Empty");
	}else{
		printf("Queue is NOT Empty");
	}
}

void Queue_Enqueue(struct queue *q){
	struct node *tmp = malloc(sizeof(node));
	assert(tmp != NULL);
	tmp->next = NULL;

	pthread_mutex_lock(&q->GlobalLock);
	q->tail->next = tmp;
	q->tail = tmp;
	pthread_mutex_unlock(&q->GlobalLock);
}

int Queue_Dequeue(struct queue *q){
	pthread_mutex_lock(&q->GlobalLock);
	struct node *tmp = q->head;
	struct node *newHead = tmp->next;

	if(newHead == NULL){
		pthread_mutex_unlock(&q->GlobalLock);
		return -1;
	}
	q->head = newHead;
	pthread_mutex_unlock(&q->GlobalLock);
	free(tmp);
	return 0;
}

void *Thread_Enqueue(void* q){
	struct queue *PassedQueue = (struct queue *)q;
	int i = 0;
	for(i = 0; i< 1000000; i++){
		Queue_Enqueue(PassedQueue);
	}
}

void *Thread_Dequeue(void* q){
	struct queue *PassedQueue = (struct queue *)q;
	while(Queue_Dequeue(PassedQueue) == 0);
}
