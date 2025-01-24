///////////////////////////////////////////////////////////////////
//
// ADT BList
//
// Abastract bidirectional list. You can access its elemets both
// ways. Also it has insersion/deletion at every point of the list
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "common_types.h"

// Virtual nodes before the first and after the final
#define LIST_BOF (ListNode)0
#define LIST_EOF (ListNode)0


typedef struct list* List;
typedef struct list_node* ListNode;


// Creates and returns a new list
// If destory_value != NULL the it calls destroy_value(value) every time an element is removed.

List list_create(DestroyFunc destroy_value);

// Returns the number of elements in the list

int list_size(List list);

// Inserts a node before the node or at the end if node == LIST_EOF

void list_insert(List list, ListNode node, Pointer value);

// Inserts a node at the correct order according to the compare function

void list_ordered_insert(List list, Pointer value, CompareFunc compare);

// Removes node (it has to be in the list).

void list_remove(List list, ListNode node);

// Returns the first value equal to Pointer value or NULL if it doesn't exists

Pointer list_find(List list, Pointer value, CompareFunc compare);

// Changes the function which is called at the removal/replacement of and element
// with destroy_value. Returns the previous value of the funciton.

DestroyFunc list_set_destroy_value(List list, DestroyFunc destroy_value);

// Frees all the memory that the list is allocating.
// Whatever you do after destroy is non defined

void list_destroy(List list);


// List traversal //////
// Returns the first or the last node of the list, of LIST_BOF/LIST_EOF if the list is empty

ListNode list_first(List list);
ListNode list_last(List list);

// Returns the next and the previous list nodes, of BLIST_EOF / BLIST_BOF
// if the node does not have next / previous.

ListNode list_next(List list, ListNode node);
ListNode list_previous(List list, ListNode node);

// Returns the content of the node.

Pointer list_node_value(List list, ListNode node);

// Finds the first element equal to balue and returns the node of the value of LIST_EOF 
// if it does not exists.

ListNode list_find_node(List list, Pointer value, CompareFunc compare);
