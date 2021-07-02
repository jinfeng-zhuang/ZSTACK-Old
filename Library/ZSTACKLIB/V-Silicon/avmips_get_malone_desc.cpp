#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

struct malone_log_desc {
    unsigned int wp;
    unsigned int rp;
    unsigned int start;
    unsigned int end;
    unsigned int lo;
    unsigned int hi;
};

int avmips_get_malone_desc(struct ringbuffer *r)
{
    int ret;
    unsigned int result;
    struct malone_log_desc desc;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_FWPRINTADDR, &result, 1);
    if (0 != ret) {
        return -1;
    }

    if (result) {
        ret = dbg_host_read8(result, (unsigned char *)&desc, sizeof(desc));
        if (0 != ret) {
            return -1;
        }

        r->start = desc.start;
        r->end = desc.end;
        r->wp = desc.wp;
        r->rp = desc.rp;

        info("Malone Log: %#X %#X WP %#08X RP %#08X: %s\n", r->start, r->end, r->wp - r->start, r->rp - r->start, (r->wp >= r->rp) ? "--->" : "<---");
    }

    return 0;
}
