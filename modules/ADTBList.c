///////////////////////////////////////////////////////////
//
// Implementation of ADT List with binary linked list.
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ADTBList.h"


// List is pointer to this struct
struct list {
	ListNode dummy;			// Even empty list has a node
	ListNode last;				// pointer to the last node or at the dummy(if list is empty)
	int size;					// list_size will be O(1)
	DestroyFunc destroy_value;	// Function that destory an element of the list
};

// ListNode is pointer to this struct
struct list_node {
	ListNode next;
	ListNode prev;
	Pointer value;
};


List list_create(DestroyFunc destroy_value) {

	List list = malloc(sizeof(*list));
	list->size = 0;
	list->destroy_value = destroy_value;

	list->dummy = malloc(sizeof(*list->dummy));
	list->dummy->next = NULL;		// empty list, dymmy does not has next
	list->dummy->prev = NULL;		// neither previous

	// In an empty list, the last node is also dummy
	list->last = list->dummy;

	return list;
}

int list_size(List list) {
	return list->size;
}

void list_insert(List list, ListNode node, Pointer value) {

	if(node == NULL){
		// Creation of the new node
		ListNode new = malloc(sizeof(*new));

		// Data insertion
		new->value = value;

		ListNode last_node;

		// If the list is empty then the last node is the dummy
		if(list->size == 0){
			last_node = list->dummy;
		}
		else{
			last_node = list_last(list);
		}

		last_node->next = new;

		new->prev = last_node;

		new->next = NULL;

		list->last = new;

		list->size++;

		list->last = new;
	}
	else{

		ListNode new = malloc(sizeof(*new));

		new->value = value;

		new->prev = node->prev;

		node->prev = new;

		new->next = node;

		new->prev->next = new;

		list->size++;
	}
}

void list_ordered_insert(List list, Pointer value, CompareFunc compare) {
	// Search for a greater value than "value" argument and insert it there so
	//that the list will be ordered
	for(ListNode lnode = list_first(list);
	lnode != LIST_EOF;
	lnode = list_next(list, lnode)) {
		if(compare(value, list_node_value(list, lnode)) < 0) {
			list_insert(list, lnode, value);
			return;
		}
	}
	// If it made it till here that means that value is the largest value in the list 
	// so I have to put value in the end. But it also may be that the list is empty 
	// so again passing LIST_EOF will make value the first item of the list
	list_insert(list, LIST_EOF, value);
}

void list_remove(List list, ListNode node) {
	assert(node != NULL);		// LCOV_EXCL_LINE

	if (list->destroy_value != NULL)
		list->destroy_value(node->value);
	// If node is the last one then its previous becomes the last node of the list
	if(node == list->last){
		node->prev->next = NULL;

		list->last = node->prev;
	}
	else{
		node->prev->next = node->next;

		node->next->prev = node->prev;
	}

	free(node);

	list->size--;
}

Pointer list_find(List List, Pointer value, CompareFunc compare) {
	ListNode node = list_find_node(List, value, compare);
	return node == NULL ? NULL : node->value;
}

DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy_value) {
	DestroyFunc old = list->destroy_value;
	list->destroy_value = destroy_value;
	return old;
}

void list_destroy(List list) {
	// List traversal then frees all the codes with dummy
	ListNode node = list->dummy;
	while (node != NULL) {
		ListNode next = node->next;

		// Calls destroy_value if it exists(not on dummy)
		if (node != list->dummy && list->destroy_value != NULL)
			list->destroy_value(node->value);

		free(node);
		node = next;
	}

	// Frees struct
	free(list);
}


// List traversal ////

ListNode list_first(List list) {
	return list->dummy->next;
}

ListNode list_last(List list) {
	// If the list is empty the last points to dummy. But we want to return NULL, not the dummy!
	if (list->last == list->dummy)
		return LIST_EOF;		// empty list
	else
		return list->last;
}

ListNode list_next(List list, ListNode node) {
	assert(node != NULL);
	return node->next;
}

ListNode list_previous(List list, ListNode node) {
	assert(node != NULL);
	return node->prev;
}

Pointer list_node_value(List list, ListNode node) {
	assert(node != NULL);	// LCOV_EXCL_LINE
	return node->value;
}

ListNode list_find_node(List list, Pointer value, CompareFunc compare) {
	// List traversal and calls compare till it returns 0
	//
	for (ListNode node = list->dummy->next; node != NULL; node = node->next)
		if (compare(value, node->value) == 0)
			return node;

	return NULL;	// It doesn't exist
}

void print_elements(List list){
	ListNode las = list_first(list);
    printf("First = %d\n", *(int*)list_node_value(list, las));
	ListNode last = list->last;
    printf("Last = %d\n",*(int*)list_node_value(list, last));
	printf("Size is %d\n",list->size);
}


void print_list(List list) {
	for(ListNode lnode = list_first(list);
	lnode != LIST_EOF;
	lnode = list_next(list, lnode)) {
		int value = (int*)list_node_value(list, lnode);
		printf("%d\t", value);
	}
}