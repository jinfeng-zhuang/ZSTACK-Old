#ifndef CARD_H
#define CARD_H

#include "card_attr.h"

#define CARD_EVENT_MATCH_MAX    (10)
#define CARD_EVENT_ATTR_MAX     (10)
#define CARD_EVENT_QUEUE_SIZE (100)

#define CARD_ATTR_MAX (100)
#define CARD_QUEUE_SIZE (100)

struct card;
struct card_msg;

typedef int (*card_msg_proc)(struct card *card, struct card_msg *msg);

struct card_attr {
    int attr;
    int value;
};

struct card {
    card_msg_proc    proc;
    struct card_attr attr[CARD_ATTR_MAX];
};

struct card_msg {
    int             sender;

    int              match_num;
    struct card_attr match[CARD_EVENT_MATCH_MAX];

    int              attr_num;
    struct card_attr attr[CARD_EVENT_ATTR_MAX];
};

struct card_db {
    int             msg_num;
    struct card_msg msg_queue[CARD_EVENT_QUEUE_SIZE];

    int             card_num;
    struct card     card_queue[CARD_QUEUE_SIZE];
};

extern struct card_db card_db;

extern int card_get(int index, struct card *c);
extern int card_add(struct card *c); // can't delete

extern int card_msg_put(struct card_msg *e);
extern int card_msg_get(struct card_msg *e);

// return a list of card id?
// index = 0
// while card_match() != -1
extern int card_match(struct card_msg *e, int *index);

extern int card_dispatch_message(struct card_msg *e);

#endif
