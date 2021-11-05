#include <stdio.h>
#include <stdlib.h>

//define single queueNode
struct node {
    int value;
    struct node* next;
};

typedef struct node QueueElement;

//define queue struct
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
    //allocates the necessary memory
    Queue* queue = (Queue*)malloc(sizeof (Queue));
    //sets its maximum length to the transferred value.
    queue->max_length = length;
    //initializes all fields with NULL.
    queue->actual_length = 0;
    queue->first=NULL;
    queue->last=NULL;
    //returns a new queue
    return queue;
}
//create a new element that stores the integer data and adds the new element to the end of the queue.
int Queue_enqueue(Queue* queue, int value){
    //If the queue is already full, a warning is issued, a negative value is returned and no new element is created and saved.
    if(queue->max_length==queue->actual_length) return -1;
    if(queue->actual_length==0){//if the queue is leer, create new first and last node.
        queue->first = (QueueElement*)malloc(sizeof (QueueElement));
        queue->first->value = value;
        queue->last = queue->first;
    }else {//if the queue is not leer
        QueueElement *queueElement = (QueueElement *) malloc(sizeof(QueueElement));
        queueElement->next = NULL;
        queueElement->value = value;
        queue->first->next = queueElement;
        queue->first = queueElement;
    }
    queue->actual_length = (queue->actual_length)+1;
    //The new length of the queue is its return value.
    return queue->actual_length;
}

//return the first item in the queue and removes it from the queue.
QueueElement* Queue_dequeue(Queue* queue){
    // If the queue is empty, NULL is returned.
    if(queue->actual_length==0) return NULL;
    QueueElement* res = queue->last;
    queue->last = queue->last->next;
    queue->actual_length--;
    return res;
}

//output the current length of the queue on the console, as well as the elements it contains, starting with front.
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
