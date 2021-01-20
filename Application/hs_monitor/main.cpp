#include <zstack.h>

#include <wchar.h>
#include <locale.h>

#include "main.h"

struct application app;

static struct list_head head;

int hs_match_and_operate(struct hs_event *evt, struct hs_card *card)
{
    struct list_head *pos;
    struct hs_attr *attr;
    int attr_id;
    int attr_val;

    list_for_each(pos, &evt->match) {
        attr = list_entry(pos, struct hs_attr, list);

        attr_id = attr->id;
        attr_val = attr->value;

        if (attr_val != card->attr[attr_id]) {
            return 0;
        }
    }

    list_for_each(pos, &evt->operation) {
        attr = list_entry(pos, struct hs_attr, list);

        attr_id = attr->id;
        attr_val = attr->value;

        card->attr[attr_id] += attr_val;
    }

    return 1;
}

struct hs_card *hs_card_init(wchar_t *name, int type, int prio, int cost, int attack, int blood)
{
    struct hs_card *card = NULL;

    card = (struct hs_card *)malloc(sizeof(struct hs_card));
    if (card) {
        memset(card, 0, sizeof(struct hs_card));

        memcpy(card->name, name, wcslen(name) * 2);
        card->attr[HS_ATTR_TYPE] = type;
        card->attr[HS_ATTR_PRIO] = prio;
        card->attr[HS_ATTR_COST] = cost;
        card->attr[HS_ATTR_ATTACK] = attack;
        card->attr[HS_ATTR_BLOOD] = blood;
    }

    return card;
}

struct hs_event *hs_event_add(struct hs_event *evt, int match_id, int match_value, int operation_id, int operation_value)
{
    struct hs_attr *attr;

    if (evt == NULL) {
        evt = (struct hs_event *)malloc(sizeof(struct hs_event));
        memset(evt, 0, sizeof(struct hs_event));
        INIT_LIST_HEAD(&evt->match);
        INIT_LIST_HEAD(&evt->operation);
    }

    if (evt) {
        attr = (struct hs_attr *)malloc(sizeof(struct hs_attr));
        if (attr) {
            memset(attr, 0, sizeof(struct hs_attr));
            attr->id = match_id;
            attr->value = match_value;
            list_add_tail(&attr->list, &evt->match);
        }

        attr = (struct hs_attr *)malloc(sizeof(struct hs_attr));
        if (attr) {
            memset(attr, 0, sizeof(struct hs_attr));
            attr->id = operation_id;
            attr->value = operation_value;
            list_add_tail(&attr->list, &evt->operation);
        }
    }

    return evt;
}

int card_dump(struct list_head *head)
{
    struct hs_card *card;
    struct list_head *pos;

    info("---------------------------------------------------------------\n");
    info("%4s%12s%12s%10ls\n", "COST", "ATTACK", "BLOOD", L"NAME");

    list_for_each(pos, head) { // 如果有优先级的概念，list for each 就得替换
        card = list_entry(pos, struct hs_card, list);
        info("%4d%12d%12d%10ls\n",
            card->attr[HS_ATTR_COST],
            card->attr[HS_ATTR_ATTACK],
            card->attr[HS_ATTR_BLOOD],
            card->name);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    struct list_head *card_pos, *event_pos;
    struct hs_card *card;
    struct hs_event *evt;
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    setlocale(LC_ALL, "");

    log_init(app.param.log_config);

    evt = hs_event_add(NULL, HS_ATTR_TYPE, HS_TYPE_RETINUE, HS_ATTR_BLOOD, -1);
    evt = hs_event_add(evt, HS_ATTR_TYPE, HS_TYPE_RETINUE, HS_ATTR_COST, 1);

    /////////////////////////////////////////////
    INIT_LIST_HEAD(&head);

    card = hs_card_init(L"小精灵", HS_TYPE_RETINUE, HS_PRIO_COMMON, 0, 1, 1);
    list_add_tail(&card->list, &head);
    card = hs_card_init(L"圣光护卫者", HS_TYPE_RETINUE, HS_PRIO_COMMON, 1, 1, 2);
    list_add_tail(&card->list, &head);
    card = hs_card_init(L"上古看守者", HS_TYPE_RETINUE, HS_PRIO_COMMON, 2, 4, 5);
    list_add_tail(&card->list, &head);
    card = hs_card_init(L"法术反制", HS_TYPE_MAGIC, HS_PRIO_COMMON, 3, 0, 0);
    list_add_tail(&card->list, &head);
    /////////////////////////////////////////////

    card_dump(&head);

    list_for_each(card_pos, &head) { // 如果有优先级的概念，list for each 就得替换
        card = list_entry(card_pos, struct hs_card, list);

        ret = hs_match_and_operate(evt, card);
        if (ret) {
            if (card->cb) {
                card->cb(evt);
            }
        }
    }

    card_dump(&head);

    return 0;
}
