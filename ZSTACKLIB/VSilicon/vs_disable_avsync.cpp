#define LOG_MODULE LOG_MODULE_AVMIPS

#include <zstack/log.h>

#include <vs/dbg.h>
#include <vs/avmips.h>

int vs_disable_avsync(void)
{
    int ret;
    unsigned int result;
    
    ret = dbg_avmips_read32(AVSYNC_MODE_DISABLE | 0<<6, &result, 1);
    if (0 != ret) {
        debug("can't get address\n");
        return -1;
    }
    
    return 0;
}
