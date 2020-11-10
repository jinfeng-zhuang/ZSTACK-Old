#include <zstack/ringbuf.h>
#include <zstack/log.h>

#include <vs/dbg.h>
#include <vs/avmips.h>

//#define LOG_MODULE LOG_MODULE_AVMIPS

#define SX6_HW_DEMUX_REG_BASE 0x15090000
#define SX6_HW_DEMUX_REG_BASE_MASK 0xFFFF0000

#define UXL_HW_DEMUX_REG_BASE 0x10019000
#define UXL_HW_DEMUX_REG_BASE_MASK 0xFFFFF000

int avmips_get_ves_desc(struct ring *r)
{
    int ret;
    unsigned int ves_addr;
    unsigned int remain;
    int flag_hardware_demux = 0;
    struct HWDemuxVESDesc_t HWDesc;
    struct AVStreamVESDesc_t AVDesc;
    int flag_dir;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_VESDESCADDR | 0<<6, &ves_addr, 1);
    if (0 != ret) {
        log(LOG_DEBUG, "can't get address\n");
        return -1;
    }

    if (ves_addr == 0) {
        log(LOG_DEBUG, "no stream playback through avmips\n");
        return -1;
    }
    
    if(((ves_addr & SX6_HW_DEMUX_REG_BASE_MASK) == SX6_HW_DEMUX_REG_BASE)
        || ((ves_addr & UXL_HW_DEMUX_REG_BASE_MASK) == UXL_HW_DEMUX_REG_BASE)
        || ((ves_addr & 0xFFFF0000) == 0x10010000))
    {
        flag_hardware_demux = 1;
    }
        
    if (flag_hardware_demux) {
        ves_addr |= 0xF5080000;
        
        dbg_host_read8(ves_addr, (unsigned char *)&HWDesc, sizeof(struct HWDemuxVESDesc_t));

        r->start = HWDesc.start;
        r->end = HWDesc.end;
        r->wp = HWDesc.wp;
        r->rp = HWDesc.rp;
    }
    else {
        dbg_host_read8(ves_addr, (unsigned char *)&AVDesc, sizeof(struct AVStreamVESDesc_t));
        
        r->start = AVDesc.start;
        r->end = AVDesc.end;
        r->wp = AVDesc.wp;
        r->rp = AVDesc.rp;
    }

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

    log(LOG_USER, "%s: %#X %#X %dMB %#X %#X, remain %dM %dK %dB, %s\n",
        flag_hardware_demux ? "HW" : "SW",
        r->start, r->end,
        (r->end - r->start + 1) >> 20,
        r->wp, r->rp,
        remain>>20 & 0x3FF,
        remain>>10 & 0x3FF,
        remain & 0x3FF,
        flag_dir ? "--->" : "<---");

    return 0;
}
