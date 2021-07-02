#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct list_head {
    struct list_head *next, *prev;
};

#define list_entry(ptr, type, member) container_of(ptr, type, member)

//#define offsetof(TYPE, MEMBER) ((size_t)& ((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) (type*)((char *)ptr - offsetof(type,member))

#if (__STDC_VERSION__ >= 199901L)
static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *newc,
    struct list_head *prev,
    struct list_head *next)
{
    next->prev = newc;
    newc->next = next;
    newc->prev = prev;
    prev->next = newc;
}

static inline void list_add(struct list_head *newc, struct list_head *head)
{
    __list_add(newc, head, head->next);
}

static inline void list_add_tail(struct list_head *newc, struct list_head *head)
{
    __list_add(newc, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

static inline void list_replace(struct list_head *old,
    struct list_head *newc)
{
    newc->next = old->next;
    newc->next->prev = newc;
    newc->prev = old->prev;
    newc->prev->next = newc;
}

static inline void list_replace_init(struct list_head *old,
    struct list_head *newc)
{
    list_replace(old, newc);
    INIT_LIST_HEAD(old);
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add(list, head);
}

static inline void list_move_tail(struct list_head *list,
    struct list_head *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}
#else
extern void INIT_LIST_HEAD(struct list_head *list);

extern void list_add(struct list_head *newc, struct list_head *head);

extern void list_add_tail(struct list_head *newc, struct list_head *head);

extern void list_del(struct list_head *entry);

extern void list_del_init(struct list_head *entry);

extern void list_replace(struct list_head *old,
    struct list_head *newc);

extern void list_replace_init(struct list_head *old,
    struct list_head *newc);

extern void list_move(struct list_head *list, struct list_head *head);

extern void list_move_tail(struct list_head *list,
    struct list_head *head);
#endif

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_prev(pos, head) \
    for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
        pos != (head); \
        pos = n, n = pos->prev)

#ifdef __cplusplus
}
#endif

#endif
