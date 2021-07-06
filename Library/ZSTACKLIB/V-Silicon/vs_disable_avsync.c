#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

int vs_disable_avsync(void)
{
    int ret;
    unsigned int result;
    
    ret = dbg_avmips_read32(AVSYNC_MODE_DISABLE | 0<<6, &result, 1);
    if (0 != ret) {
        warn("can't get address\n");
        return -1;
    }
    
    return 0;
}
