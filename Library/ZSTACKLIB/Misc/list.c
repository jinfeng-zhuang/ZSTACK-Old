#include <zstack/list.h>
#include <stddef.h>

void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

void __list_add(struct list_head *newc,
    struct list_head *prev,
    struct list_head *next)
{
    next->prev = newc;
    newc->next = next;
    newc->prev = prev;
    prev->next = newc;
}

void list_add(struct list_head *newc, struct list_head *head)
{
    __list_add(newc, head, head->next);
}

void list_add_tail(struct list_head *newc, struct list_head *head)
{
    __list_add(newc, head->prev, head);
}

void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = NULL;
    entry->prev = NULL;
}

void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

void list_replace(struct list_head *old,
    struct list_head *newc)
{
    newc->next = old->next;
    newc->next->prev = newc;
    newc->prev = old->prev;
    newc->prev->next = newc;
}

void list_replace_init(struct list_head *old,
    struct list_head *newc)
{
    list_replace(old, newc);
    INIT_LIST_HEAD(old);
}

void list_move(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add(list, head);
}

void list_move_tail(struct list_head *list,
    struct list_head *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}