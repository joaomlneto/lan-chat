/*
 * Thread-Safe Queue
 *
 * queue.h
 *
 * A Thread-Safe FIFO queue implementation using double linked lists
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

queue_t* queue_create() {
	queue_t *queue;
	queue = malloc(sizeof(queue_t));
	queue->first = NULL;
	queue->last = NULL;
	queue->length = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	return queue;
}

void queue_destroy(queue_t* queue) {
	if (queue == NULL) {
		printf("[queue_destroy] Tried to destroy a 'NULL' queue\n");
		return;
	}
	pthread_mutex_lock(&queue->mutex);
	if (queue->length != 0) {
		printf("[queue_destroy] The queue is not empty - aborting destruction.\n");
		pthread_mutex_unlock(&queue->mutex);
		return;
	}
	pthread_mutex_destroy(&queue->mutex);
	free(queue);
	return;
}

void queue_add(queue_t* queue, void* element) {
	queue_element_t* new_element;
	if (queue == NULL || element == NULL) {
		printf("[queue_add] Invalid queue or element\n");
		return;
	}
	pthread_mutex_lock(&queue->mutex);
	new_element = malloc(sizeof(queue_element_t));
	new_element->element = element;
	new_element->next = NULL;
	if (queue->first == NULL) {
		queue->first = new_element;
		queue->last = new_element;
	}
	else {
		queue->last->next = new_element;
		queue->last = new_element;
	}
	queue->length++;
	pthread_mutex_unlock(&queue->mutex);
}

size_t queue_length(queue_t* queue) {
	return queue->length;
}

void* queue_first(queue_t *queue) {
	pthread_mutex_lock(&queue->mutex);
	if ((queue == NULL) || (queue->first == NULL)) {
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	pthread_mutex_unlock(&queue->mutex);
	return queue->first->element;
}

void* queue_remove(queue_t* queue) {
	queue_element_t* node;
	pthread_mutex_lock(&queue->mutex);
	if ((queue == NULL) || (queue->first == NULL)) {
		pthread_mutex_unlock(&queue->mutex);
		return NULL;
	}
	node = queue->first;
	if (queue->first == queue->last) {
		queue->first = NULL;
		queue->last = NULL;
	}
	else {
		queue->first = node->next;
	}
	queue->length--;
	void* element = node->element;
	free(node);
	pthread_mutex_unlock(&queue->mutex);
	return element;
}

