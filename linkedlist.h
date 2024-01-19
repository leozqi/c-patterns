#ifndef COMMON_LINKEDLIST
#define COMMON_LINKEDLIST

#include "common.h"
#include "err.h"

typedef struct node {
	int data;
	struct node *p_next;
} node_t;

node_t *linkedlist_create(int val)
{
	node_t *p_node = NULL;
	p_node = (node_t *) malloc(sizeof(node_t));

	if (p_node == NULL) {
		common_err_alloc();
	}

	p_node->data = val;
	p_node->p_next = NULL;

	return p_node;
}

void linkedlist_push(node_t **p_head, int data)
{
	node_t *p_new = linkedlist_create(data);
	p_new->p_next = (*p_head);
	(*p_head) = p_new;
}

void linkedlist_append(node_t *p_head, int data)
{
	node_t *p_curr = p_head;

	while (p_curr->next != NULL) {
		p_curr = p_curr->p_next;
	}

	// End of list
	p_curr->p_next = linkedlist_create(data);
}

int linkedlist_pop(node_t **p_head)
{
	int retval = -1;

	if (*p_head == NULL) {
		return retval;
	}

	node_t *p_next = (*p_head)->p_next;
	retval = (*p_head)->data;

	free(*p_head);

	*p_head = p_next;
	return retval;
}

int linkedlist_remove_last(node_t *p_head)
{
	int retval = -1;

	if (p_head == NULL) {
		return retval;
	} else if (p_head->p_next == NULL) {
		// only one item in list: remove
		retval = p_head->data;
		free(p_head);
		return retval;
	}

	node_t *p_curr = p_head;
	// Second-to-last node in list
	while (p_curr->p_next->p_next != NULL) {
		p_curr = p_curr->p_next;
	}

	retval = p_curr->p_next->data;
	free(p_curr->p_next);
	p_curr->p_next = NULL;
	return retval;
}

int linkedlist_remove_index(node_t **p_head, int n)
{
	int retval = -1;
	node_t *p_curr = *p_head;
	node_t *p_tmp = NULL;

	if (n == 0)
		return linkedlist_pop(p_head);

	for (int i = 0; i < n-1; i++) {
		if (p_curr->p_next == NULL)
			return retval;

		p_curr = p_curr->next;
	}

	if (p_curr->next == NULL)
		return retval;

	p_tmp = p_curr->p_next;
	retval = p_tmp->data;
	p_curr->p_next = p_tmp->p_next;
	free(p_tmp);

	return retval;
}

void linkedlist_walk(node_t *p_head)
{
	node_t *p_curr = p_head;

	while (p_curr != NULL) {
		// manipulate p_curr
		p_curr = p_curr->p_next;
	}
}

#endif // ifndef COMMON_LINKEDLIST
