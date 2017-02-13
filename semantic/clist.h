#ifndef __CLIST_H__
#define __CLIST_H__

// using macro to generate different type of list
#include <string.h>
#include <stdlib.h>

#define DEFINE_LIST_NODE(type) \
	struct list_node_##type { \
		struct list_node_##type *next; \
		type data; \
	}; \
	typedef struct list_node_##type list_node_##type;

#define EXTERN_LIST_PUSH_FRONT(type) \
	void list_push_front_##type(list_node_##type **h, type* d);

#define IMPLEMENT_LIST_PUSH_FRONT(type) \
	void list_push_front_##type(list_node_##type **h, type* d) { \
		list_node_##type *newnode = (list_node_##type*)malloc(sizeof(list_node_##type)); \
		memcpy(&(newnode->data), d, sizeof(type)); \
		newnode->next = *h; \
		*h = newnode; \
	}

#define EXTERN_LIST_PUSH_BACK(type) \
	void list_push_back_##type(list_node_##type **h, type* d);

#define IMPLEMENT_LIST_PUSH_BACK(type) \
	void list_push_back_##type(list_node_##type **h, type* d) { \
		list_node_##type *newnode = (list_node_##type*)malloc(sizeof(list_node_##type)); \
		memcpy(&(newnode->data), d, sizeof(type)); \
		newnode->next = NULL; \
		if(*h == NULL) { \
			*h = newnode; \
			return; \
		} else { \
			list_node_##type *pre = NULL, *p = *h; \
			while(p != NULL) { \
				pre = p; \
				p = p->next; \
			} \
			pre->next = newnode; \
		} \
	}

#define EXTERN_LIST_MERGE(type) \
	void list_merge_##type(list_node_##type **h1, list_node_##type *h2);

#define IMPLEMENT_LIST_MERGE(type) \
	void list_merge_##type(list_node_##type **h1, list_node_##type *h2) { \
		if(*h1 == NULL) { \
			*h1 = h2; \
			return; \
		} else { \
			list_node_##type *pre = NULL, *p = *h1; \
			while(p != NULL) { \
				pre = p; \
				p = p->next; \
			} \
			pre->next = h2; \
		} \
	}


#define IMPLEMENT_LIST_MATCH(type, begin, size) \
	int list_match_##type(type *d1, void *d2) { \
		return memcmp(d1 + begin, d2, size); \
	}

#define EXTERN_LIST_FIND(type) \
	list_node_##type* list_find_##type(list_node_##type *h, void *d);

#define IMPLEMENT_LIST_FIND(type) \
	list_node_##type* list_find_##type(list_node_##type *h, void *d) { \
		list_node_##type *tmp = h; \
		while(tmp != NULL) { \
			if(list_match_##type(&(tmp->data), d) == 0) \
				return tmp; \
			tmp = tmp->next; \
		} \
		return NULL; \
	}

#define EXTERN_LIST_FREE(type) \
	void list_free_##type(list_node_##type **h);

#define IMPLEMENT_LIST_FREE(type) \
	void list_free_##type(list_node_##type **h) { \
		list_node_##type *tmp; \
		while(*h != NULL) { \
			tmp = (*h)->next; \
			free(*h); \
			*h = tmp; \
		} \
	}


#endif /* __CLIST_H__ */
