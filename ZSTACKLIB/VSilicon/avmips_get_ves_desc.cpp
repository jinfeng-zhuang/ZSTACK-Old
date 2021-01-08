#define LOG_MODULE LOG_MODULE_AVMIPS

#include <zstack/ringbuf.h>
#include <zstack/log.h>

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
    
    ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_VESDESCADDR | (channel<<6), &ves_addr, 1);
    if (0 != ret) {
        debug("can't get address\n");
        return -1;
    }

    if (ves_addr == 0) {
        if (vs_ves_descriptor_address != 0) {
            ves_addr = vs_ves_descriptor_address;
        }
        else {
            warn("ves descriptor address is NULL!\n");
            return -1;
        }
    }
    else {
        vs_ves_descriptor_address = ves_addr;
        //log_info("ves_address = %#8x\n", ves_addr);
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

    info("[CH%d][%s] %#8X %#8X RD: %#8X WR: %#8X, remain %2dM %4dK %4dB, %s\n",
        channel,
        flag_hardware_demux ? "HW" : "SW",
        r->start, r->end,
        r->rp - r->start, r->wp - r->start, 
        remain>>20 & 0x3FF,
        remain>>10 & 0x3FF,
        remain & 0x3FF,
        flag_dir ? "--->" : "<---");

    return 0;
}
