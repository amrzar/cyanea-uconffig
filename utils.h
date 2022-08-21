/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>

#undef offsetof
#define offsetof(type, member) ((size_t)&((type *)0)->member)

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

/*
 * Circular doubly linked list 'struct list_head'.
 *
 **/

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }
static inline void INIT_LIST_HEAD(struct list_head *entry) {
    entry->next = entry->prev = entry;
}

static inline void list_add(struct list_head *new,
    struct list_head *prev, struct list_head *next) {

    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    list_add(new, head->prev, head);
}

#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*(pos)), member); \
        &pos->member != (head); \
        pos = container_of((pos)->member.next, typeof(*(pos)), member))

/*
 * Double linked lists 'struct hlist_node' with a single pointer
 * list head 'struct hlist_head' for hash table.
 *
 **/

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, * *pprev;
};

#define HLIST_HEAD_INIT { .first = NULL }
#define INIT_HLIST_HEAD(head) ((head)->first = NULL)
static inline void INIT_HLIST_NODE(struct hlist_node *entry) {
    entry->next = NULL;
    entry->pprev = NULL;
}

static inline void hlist_add_head(struct hlist_node *new, struct hlist_head *head) {
    struct hlist_node *first = head->first;
    new->next = first;

    if (first != NULL)
        first->pprev = &new->next;

    head->first = new;
    new->pprev = &head->first;
}

#define hlist_entry_safe(ptr, type, member) \
    ({ typeof(ptr) ____ptr  = (ptr); \
        ____ptr  ? container_of(____ptr , type, member) : NULL; \
    })

#define hlist_for_each_entry(pos, head, member) \
    for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member); \
        pos; \
        pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

#define debug_print(...) fprintf(stderr, "[debug_print]" __VA_ARGS__)
#define error_print(...) fprintf(stderr, "[error_print]" __VA_ARGS__)

#endif /* __UTILS_H__ */