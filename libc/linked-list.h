#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/*
 * 1 2 3 "text"
 * Singly linked list:
 * Doubly linked list:
 *
 */

struct node {
	int value;
	struct node *next;
};

struct node *linked_list_create(int value);
struct node *linked_list_append(int value);

#endif LINKED_LIST_H
