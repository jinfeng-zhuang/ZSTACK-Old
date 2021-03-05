#include <zstack.h>

#include <wchar.h>
#include <locale.h>

#include "main.h"

struct application app;

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

    /////////////////////////////////////////////

    INIT_LIST_HEAD(&app.event_queue);
    INIT_LIST_HEAD(&app.card_queue);

    /////////////////////////////////////////////

    hs_card_dump(&app.card_queue);

    // translate the card to event, the match => dst
    // operation to event

    list_for_each(event_pos, &app.event_queue) {
        evt = list_entry(event_pos, struct hs_event, list);

        // user proc
        list_for_each(card_pos, &app.card_queue) {
            card = list_entry(card_pos, struct hs_card, list);
        }

        // change attr
        /*list_for_each(card_pos, &evt->dst) {
            card = list_entry(card_pos, struct hs_card, list);

            // operation on the attr
            if (evt->operation.operate == 0) { // + -
                card->attr[evt->operation.id] += evt->operation.value[0];
            }
        }*/

        // user proc
        list_for_each(card_pos, &app.card_queue) {
            card = list_entry(card_pos, struct hs_card, list);
        }
    }

    return 0;
}
