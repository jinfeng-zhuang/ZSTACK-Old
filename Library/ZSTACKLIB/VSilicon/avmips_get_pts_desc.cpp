#define LOG_MODULE LOG_MODULE_AVMIPS

#include <stdlib.h>

#include <zstack/ringbuf.h>
#include <zstack/log.h>
#include <zstack/misc.h>

#include <vs/dbg.h>
#include <vs/avmips.h>

#define SX6_HW_DEMUX_REG_BASE 0x15090000
#define SX6_HW_DEMUX_REG_BASE_MASK 0xFFFF0000

#define UXL_HW_DEMUX_REG_BASE 0x10019000
#define UXL_HW_DEMUX_REG_BASE_MASK 0xFFFFF000

unsigned char *avmips_dump_pts_queue(int channel)
{
    int ret;
    unsigned int ves_addr;
    struct VideoPTSQueue_t *queue;

    queue = (struct VideoPTSQueue_t *)malloc(sizeof(struct VideoPTSQueue_t *));
    if (NULL == queue)
        goto FAILED;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_PTSDESCADDR | channel<<6, &ves_addr, 1);
    if (0 != ret) {
        debug("can't get address\n");
        goto FAILED;
    }

    if (ves_addr == 0) {
        debug("can get address from avmips\n");
        goto FAILED;
    }

    ret = dbg_host_read8(ves_addr, (unsigned char *)queue, sizeof(struct VideoPTSQueue_t));
    if (0 != ret) {
        goto FAILED;
    }

    return (unsigned char *)queue;

FAILED:
    if (queue)
        free(queue);
    return NULL;
}

int avmips_get_pts_desc(struct ring *r, int channel)
{
    int ret;
    unsigned int ves_addr;
    unsigned int remain;
    int flag_dir;
    struct VideoPTSQueue_t_Part part;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_PTSDESCADDR | channel<<6, &ves_addr, 1);
    if (0 != ret) {
        warn("can't get address\n");
        return -1;
    }

    if (ves_addr == 0) {
        warn("address from avmips is NULL\n");
        return -1;
    }

    dbg_host_read8(ves_addr + OFFSET(struct VideoPTSQueue_t, sz), (unsigned char *)&part, sizeof(struct VideoPTSQueue_t_Part));
    
    r->start = 0;
    r->end = part.size;
    r->wp = part.wr;
    r->rp = part.rd;

    if (r->wp >= r->rp) {
        flag_dir = 1;
    }
    else {
        flag_dir = 0;
    }

    if (flag_dir) {
        remain = r->wp - r->rp;
    }
    else {
        remain = (r->end - r->start + 1) - (r->rp - r->wp);
    }

    log_info("%#08X: %#08X %#08X %dMB WP %#08X RP %#08X, remain %dM %dK %dB, %s\n",
        ves_addr,
        r->start, r->end,
        (r->end - r->start + 1) >> 20,
        r->wp, r->rp,
        remain>>20 & 0x3FF,
        remain>>10 & 0x3FF,
        remain & 0x3FF,
        flag_dir ? "--->" : "<---");

    return 0;
}
