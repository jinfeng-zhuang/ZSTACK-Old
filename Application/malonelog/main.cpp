#include <zstack.h>
#include <vs.h>

#include "main.h"

extern int avmips_get_version(void);
extern int avmips_get_malone_desc(struct ring *r);
extern int hexdump_ascii(void* start, unsigned int length);
extern int ascii_dump(void* start, unsigned int length);

struct application app;

int main(int argc, char *argv[])
{
    int ret;
    struct ring desc;
    unsigned char *buffer;
    int keep;
    int len;
    int rp;
    int i;

    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    ret = dbg_init(app.param.ip);
    if (0 != ret) {
        return -1;
    }

    avmips_get_version();

    avmips_get_malone_desc(&desc);

    ret = vs_pman_enable((sx_chip)vs_chip_id_get(), (enum sx_security_group)PMAN_SECURITY_GROUP_MVED, desc.start);
    if (0 != ret) {
        return -1;
    }

    buffer = (unsigned char *)malloc(desc.end - desc.start);
    if (NULL == buffer)
        return -1;
    memset(buffer, 0, desc.end - desc.start);

    // init rp
    rp = desc.start;

    while (1) {
        avmips_get_malone_desc(&desc);
        desc.rp = rp; // force it

        memset(buffer, 0, desc.end - desc.start);

        if (desc.wp > desc.rp) {
            len = desc.wp - desc.rp;
            dbg_host_read8(desc.rp, buffer, desc.wp - desc.rp);
        }
        else if (desc.wp == desc.rp) {
            len = 0;
        }
        else  {
            len = desc.end - desc.rp;
            dbg_host_read8(desc.rp, buffer, desc.end - desc.rp);
            keep = desc.end - desc.rp;
            len += desc.wp - desc.start;
            dbg_host_read8(desc.rp, &buffer[keep], desc.wp - desc.start);
        }

        rp = desc.wp;

        if (len) {
            //printf("len = %d\n", len);
            
            
            for (i = 0; i < len; i++) {
                if (buffer[i] == 0xc) {
                    printf("%s", &buffer[i + 4]);
                }
            }
            fflush(stdout);
        }

        Sleep(100);
    }

    return 0;
}
