#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node* next;
};

typedef struct node QueueElement;

typedef struct {
    QueueElement* first;
    QueueElement* last;
    int max_length;
    int actual_length;
} Queue;

Queue* Queue_init(int length );
int Queue_enqueue(Queue* queue, int value);
QueueElement* Queue_dequeue(Queue* queue);
void Queue_print(Queue* queue);

int main() {
	Queue* my_queue = Queue_init(5);
	QueueElement* e = Queue_dequeue(my_queue);
	if (e != NULL) free(e);

	Queue_enqueue(my_queue, 1);
	Queue_enqueue(my_queue, 2);
	Queue_enqueue(my_queue, 3);
	e = Queue_dequeue(my_queue);
	if(e!=NULL) free(e);
	Queue_enqueue(my_queue, 4);
	Queue_enqueue(my_queue, 5);
	e = Queue_dequeue(my_queue);
	free(e);
	Queue_enqueue(my_queue, 6);
	Queue_print(my_queue);

	return 0;
}
Queue* Queue_init(int length){
    Queue* queue = (Queue*)malloc(sizeof (Queue));
    queue->max_length = length;
    queue->actual_length = 0;
    queue->first=NULL;
    queue->last=NULL;
    return queue;
}
int Queue_enqueue(Queue* queue, int value){
    if(queue->max_length==queue->actual_length) return -1;
    if(queue->actual_length==0){
        queue->first = (QueueElement*)malloc(sizeof (QueueElement));
        queue->first->value = value;
        queue->last = queue->first;
    }else {
        QueueElement *queueElement = (QueueElement *) malloc(sizeof(QueueElement));
        queueElement->next = NULL;
        queueElement->value = value;
        queue->first->next = queueElement;
        queue->first = queueElement;
    }
    queue->actual_length = (queue->actual_length)+1;

    return queue->actual_length;
}
QueueElement* Queue_dequeue(Queue* queue){
    if(queue->actual_length==0) return NULL;
    QueueElement* res = queue->last;
    queue->last = queue->last->next;
    queue->actual_length--;
    return res;

}
void Queue_print(Queue* queue){
    int length = queue->actual_length;
    printf("Queue of size %d: ", length);
    QueueElement* e = queue->last;
    for(int i = 0; i<length;i++){
        printf("%d ", e->value);
        e =e->next;
    }
    printf("\n");
}
