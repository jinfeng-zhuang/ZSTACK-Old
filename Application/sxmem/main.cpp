#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#include <3rdparty/getopt.h>

#include <zstack.h>

#include <vs/sx.h>
#include <vs/dbg.h>
#include <vs/sx.h>
#include <vs/avmips.h>

unsigned int pman_umac_addr[3] = {
    0xF5005000, 0xF5008000, 0xF5036000
};

void print_usage(void)
{
}

int main(int argc, char* argv[])
{
    int ret;
    int opt;
    unsigned int id;
    char *ip;
    char *debug_config = NULL;
    unsigned int address;
    unsigned int size;
    unsigned char buffer[1024];

    if (argc < 2) {
        print_usage();
        return -1;
    }

    while ((opt = getopt(argc, argv, "m:p:wd:")) != -1)  {
        switch (opt) {
            case 'm':
                sscanf(optarg, "%x:%d", &address, &size);
                break;
            case 'd':
                debug_config = optarg;
                break;
            default:
                log(LOG_WARNING, "Unknown option: %c\n", opt);
                print_usage();
                return -1;
        }
    }

    ip = argv[optind];

    if (size > 1024)
        return -1;

    log_init(debug_config);
    
    ret = dbg_init(ip);
    if (0 == ret) {

        // Get Chip ID
        id = vs_chip_id_get();

        if ((id != CHIP_SX7B) && (id != CHIP_SX7A) && (id != CHIP_SX8B) && (id != CHIP_SX8A)) {
            log(LOG_WARNING, "CHIP Not supported\n");
            goto END;
        }

        ret = dbg_host_read8(address, buffer, size);
        if (0 == ret) {
            hexdump(buffer, size);
        }
    }

END:

    dbg_deinit();

    return 0;
}
