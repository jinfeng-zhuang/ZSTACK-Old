#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>
#include <string.h>

static unsigned int log_base;
static char msg[256];
static struct avmips_log_desc desc;

unsigned int avmips_get_log_desc(void)
{
    int ret;
    unsigned int result;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_PRINTMSGADDR, &result, 1);
    if (0 != ret) {
        return -1;
    }

    log_base = result;

    if (result) {
        ret = dbg_host_read8(result, (unsigned char *)&desc, 5 * 4);
        if (0 != ret) {
            return -1;
        }
    }

    info("enable %d total %d wr %d\n", desc.enable, desc.total, desc.wr);

    return desc.wr;
}

char *avmips_get_log(unsigned int index)
{
    int ret;

    index = (index & (512 - 1));
    
    memset(msg, 0, sizeof(msg));
    ret = dbg_host_read8(log_base + (5 * 4) + (index * 256), (unsigned char *)msg, 256);
    if (0 != ret) {
        return NULL;
    }

    return msg;
}

int avmips_log_init(char *setting)
{
    int ret;
    unsigned int result;

    info("setting: %s\n", setting);
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_PRINTSET, &result, 1);
    if (0 != ret) {
        return -1;
    }

    ret = dbg_host_write8(result, (unsigned char *)setting, (u32)strlen(setting));
    if (0 != ret) {
        return -1;
    }

    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_PRINTPARSE, &result, 1);
    if (0 != ret) {
        return -1;
    }

    return 0;
}