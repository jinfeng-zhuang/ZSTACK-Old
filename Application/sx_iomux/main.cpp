#include <zstack.h>
#include <vs.h>
#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    int chip_id;
    unsigned char value[1];
    int ret;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    if (0 != dbg_init(app.param.ip))
        return -1;

#if 0
    chip_id = vs_chip_id_get();
    if ((chip_id != CHIP_SX7B) && (chip_id != CHIP_SX7A) && (chip_id != CHIP_SX8B) && (chip_id != CHIP_SX8A)) {
        log(LOG_WARNING, "CHIP Not supported\n");
        return -1;
    }
#endif

    log(LOG_USER, "SET\n");
    
    if (app.param.uart_share == SX_AVMIPS) {
        value[0] = 0x11;
        ret = dbg_host_write8(SX_IOSHARE_UART, value, 1);
    }
    else if (app.param.uart_share == SX_ARM) {
        value[0] = 0x00;
        ret = dbg_host_write8(SX_IOSHARE_UART, value, 1);
    }

    if (0 != ret) {
        log(LOG_WARNING, "failed to write value\n");
    }

    return 0;
}
