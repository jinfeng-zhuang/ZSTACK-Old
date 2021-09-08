#include <zstack/zstack.h>

#include "main.h"
#include "card.h"

struct application app;

int main(int argc, char *argv[])
{
    struct card_msg msg;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    // init cards & message
    
    // add card

    // add msg

    while (1) {
        card_msg_get(&msg);
        card_dispatch_message(&msg);
    }

    return 0;
}
