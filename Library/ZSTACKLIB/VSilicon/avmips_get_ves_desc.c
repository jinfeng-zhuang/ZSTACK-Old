#define LOG_MODULE LOG_MODULE_AVMIPS

#include <zstack/ringbuf.h>
#include <zstack/log.h>
#include <zstack/misc.h>

#include <vs/dbg.h>
#include <vs/avmips.h>

#define SX6_HW_DEMUX_REG_BASE 0x15090000
#define SX6_HW_DEMUX_REG_BASE_MASK 0xFFFF0000

#define UXL_HW_DEMUX_REG_BASE 0x10019000
#define UXL_HW_DEMUX_REG_BASE_MASK 0xFFFFF000

static inline unsigned int K01UPA2DA(unsigned int pa)
{
	unsigned int da = pa;
	unsigned int offset = 0x30000000;	//2'b x 256M
	if((pa & 0x40000000) && !(pa & 0x80000000))	/* [1G, 2G) */
	{
		da -= offset;
	}
	return da;
}

int avmips_get_ves_desc(struct ring *r, unsigned int channel)
{
    int ret;
    unsigned int ves_addr;
    unsigned int remain;
    int flag_hardware_demux = 0;
    struct HWDemuxVESDesc_t HWDesc;
    struct AVStreamVESDesc_t AVDesc;
    int flag_dir;
    int speed_es = 0;
    int speed_decoder = 0;

    unsigned int prev_wp = 0, prev_rp = 0, prev_ms = 0, cur_ms = 0;
    unsigned int speed_wp, speed_rp;
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_VESDESCADDR | (channel<<6), &ves_addr, 1);
    if (0 != ret) {
        debug("can't get address\n");
        return -1;
    }

    if (ves_addr == 0) {
        warn("ves descriptor address is NULL!\n");
        return -1;
    }
    
    if(((ves_addr & SX6_HW_DEMUX_REG_BASE_MASK) == SX6_HW_DEMUX_REG_BASE)
        || ((ves_addr & UXL_HW_DEMUX_REG_BASE_MASK) == UXL_HW_DEMUX_REG_BASE)
        || ((ves_addr & 0xFFFF0000) == 0x10010000))
    {
        flag_hardware_demux = 1;
    }

    // TODO
    if (flag_hardware_demux) {
        ves_addr |= 0xF5080000;
        
        dbg_host_read8(ves_addr, (unsigned char *)&HWDesc, sizeof(struct HWDemuxVESDesc_t));

        r->start = HWDesc.start;
        r->end = HWDesc.end;
        r->wp = HWDesc.wp & 0x3FFFFFFF;
        r->rp = HWDesc.rp & 0x3FFFFFFF;
    }
    else {
        dbg_host_read8(ves_addr, (unsigned char *)&AVDesc, sizeof(struct AVStreamVESDesc_t));
        
        r->start = K01UPA2DA(AVDesc.start);
        r->end = K01UPA2DA(AVDesc.end + 1);
        r->wp = K01UPA2DA(AVDesc.wp);
        r->rp = K01UPA2DA(AVDesc.rp);
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

    ///////////////////////////////////////////////////////////////////////////

    cur_ms = get_ms();

    if (prev_wp < r->wp) {
        speed_wp = (r->wp - prev_wp) / (cur_ms - prev_ms);
    }
    else {
        speed_wp = (r->wp - r->start + r->end - prev_wp) / (cur_ms - prev_ms);
    }

    if (prev_rp < r->rp) {
        speed_rp = (r->rp - prev_rp) / (cur_ms - prev_ms);
    }
    else {
        speed_rp = (r->rp - r->start + r->end - prev_rp) / (cur_ms - prev_ms);
    }

    prev_wp = r->wp;
    prev_rp = r->rp;
    prev_ms = cur_ms;

    ///////////////////////////////////////////////////////////////////////////

    info("[CH%d][%s] %#8X %#8X RD: %#08X (%dBPS) WR: %#08X (%dBPS), Remain %2dM %4dK %4dB, %s\n",
        channel,
        flag_hardware_demux ? "HW" : "SW",
        r->start, r->end,
        r->rp - r->start, speed_rp / 1000,
        r->wp - r->start, speed_wp / 1000,
        remain>>20 & 0x3FF,
        remain>>10 & 0x3FF,
        remain & 0x3FF,
        flag_dir ? "--->" : "<---");

    return 0;
}
