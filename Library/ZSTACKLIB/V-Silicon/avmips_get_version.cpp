#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

int avmips_get_version(void)
{
    int ret;
    unsigned int result;
    char version[64]; // fixed value for version info
    
    ret = dbg_avmips_read32(GET_AVMIPS_VERSION, &result, 1);
    if (0 != ret) {
        return -1;
    }

    if (result) {
        ret = dbg_host_read8(result, (unsigned char *)version, sizeof(version));
        if (0 != ret) {
            return -1;
        }

        info("AVMIPS Version: %s\n", version);
    }
    else  {
        info("AVMIPS Version: not supported\n");
    }

    return 0;
}
