#include "main.h"
#include "card.h"

// call sub default proc to process all the attribute actions
static int card_default_proc(struct card* c, struct card_msg* m) {

    // for all the msg
    // switch msg type
    // call related default proc to proc the card info, add new msg

    return 0;
}

int card_dispatch_message(struct card_msg* e) {
    int index = 0;
    struct card card;

    while (card_match(e, &index)) {
        card_get(index, &card);

        if (card.proc) {
            card.proc(&card, e);
        }

        card_default_proc(&card, e);
    }

    return 0;
}