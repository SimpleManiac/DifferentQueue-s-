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
} queue;

void Queue_Init(struct queue *q);
void Queue_Enqueue(struct queue *q);
int Queue_Dequeue( struct queue *q);
void isEmpty(struct queue *q);
void* Thread_EnDequeue(void *q);


int main(){
	
	struct queue MainQueue;
	Queue_Init(&MainQueue);
	
	pthread_t enQ;
	pthread_t deQ;
	
	pthread_create(&enQ, NULL, Thread_EnDequeue, (void*) &MainQueue);	
	pthread_create(&deQ, NULL, Thread_EnDequeue, (void*) &MainQueue);
	
	pthread_join(enQ,NULL);
	pthread_join(deQ,NULL);
	
	isEmpty(&MainQueue);
}

void Queue_Init(struct queue *q){
	struct node *tmp = malloc(sizeof(node));
	tmp->next = NULL;
	q->head = tmp;
	q->tail = tmp;
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

	q->tail->next = tmp;
	q->tail = tmp;
}

int Queue_Dequeue(struct queue *q){
	struct node *tmp = q->head;
	struct node *newHead = tmp->next;

	if(newHead == NULL){
		return -1;
	}
	q->head = newHead;
	free(tmp);
	return 0;
}

void *Thread_EnDequeue(void* q){
	struct queue *PassedQueue = (struct queue *)q;
	int i = 0;
	for(i = 0; i< 1000000; i++){
		Queue_Enqueue(PassedQueue);
	}
	int j = 0;
	for(j = 0; j< 1000000; j++){
		Queue_Dequeue(PassedQueue);
	}
}

