#include "includes.h"

struct child *new_child(struct child *current, pid_t pid) {
	current->next = malloc(sizeof(struct child));
	memset(current->next, 0, sizeof(struct child));

	current->next->prev = current;
	current = current->next;
	current->pid = pid;
	current->in_syscall = 1;	/* newly created child obviously cann't be on syscall-exit */

	return current;
};

struct child *first_child(struct child *current) {
	while( current && current->prev ) current = current->prev;
	return current;
};

struct child *last_child(struct child *current) {
	while( current && current->next ) current = current->next;
	return current;
};

struct child *find_child(struct child *current, pid_t pid) {
	current = first_child(current);
	while( current )
		if( current->pid == pid ) return current;
		else if( current->next ) current = current->next;
		else break;
	return NULL;
};

void c_dup(struct child *c) {
	c = first_child(c);
	do {
		printf("[%d] 0x%08lx -> ", c->pid, (long unsigned int)c);
	} while( (c=c->next) );
	printf("\n");
};
