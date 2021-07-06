#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

int avmips_reset_decoder(int channel)
{
    int ret;
    unsigned int value;
    
    ret = dbg_avmips_read32(DEBUG_RESET_DECODER | channel<<6, &value, 1);
    if (0 != ret) {
        warn("can't get address\n");
        return -1;
    }

    return 0;
}
