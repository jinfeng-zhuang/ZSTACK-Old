#define _CRT_SECURE_NO_WARNINGS
#define ENABLE_LOG

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <Windows.h>

#include <3rdparty/getopt.h>

#include <zstack/net.h>
#include <zstack/log.h>
#include <zstack/misc.h>

#include <vs/dbg.h>
#include <vs/sx.h>
#include <vs/avmips.h>

int worker_thread(int argc, char* argv[])
{
    int ret;
    int opt;
    unsigned int id;
    char filename[64];
    char *ip;
    unsigned int ves_addr;
    int flag_loop = 0;
    int flag_dump = 0;
    struct HWDemuxVESDesc_t HWDesc;
    struct AVStreamVESDesc_t Desc;
    int flag_dir = 0;
    unsigned int remain;

    if (argc < 2) {
        print_usage();
        return -1;
    }

    while ((opt = getopt(argc, argv, "ld:")) != -1)  {
        switch (opt) {
            case 'l':
                flag_loop = 1;
                break;
            case 'd':
                flag_dump = 1;
                break;
            default:
                print_usage();
                return -1;
        }
    }

    ip = argv[optind];
    
    ret = dbg_init(ip, (unsigned short)65528);
    if (0 == ret) {

        // Get Chip ID
        ret = dbg_host_read32(0xF500E000, &id, 1);
        if (0 == ret) {
            log(LOG_USER, "Chip: %s\n", chip_name(id));
        }

        if ((chip_id(id) != CHIP_SX7B) && (chip_id(id) != CHIP_SX7A) && (chip_id(id) != CHIP_SX8B) && (chip_id(id) != CHIP_SX8A)) {
            log(LOG_WARNING, "Not supported\n");
            goto END;
        }

        ret = dbg_avmips_read32(GET_VIDEO_DEBUG_INFO_VESDESCADDR | 0<<6, &ves_addr, 1);
        if (0 != ret) {
            goto END;
        }

#define SX6_HW_DEMUX_REG_BASE 0x15090000
#define SX6_HW_DEMUX_REG_BASE_MASK 0xFFFF0000

#define UXL_HW_DEMUX_REG_BASE 0x10019000
#define UXL_HW_DEMUX_REG_BASE_MASK 0xFFFFF000

        int flag_hardware_demux = 0;

		if(((ves_addr & SX6_HW_DEMUX_REG_BASE_MASK) == SX6_HW_DEMUX_REG_BASE)
			|| ((ves_addr & UXL_HW_DEMUX_REG_BASE_MASK) == UXL_HW_DEMUX_REG_BASE)
			|| ((ves_addr & 0xFFFF0000) == 0x10010000))
		{
            flag_hardware_demux = 1;
        }
        
        while (1) {
            if (flag_hardware_demux) {
                ves_addr |= 0xF5080000;
                
                dbg_host_read8(ves_addr, (unsigned char *)&HWDesc, sizeof(struct HWDemuxVESDesc_t));

                Desc.start = HWDesc.wp;
                Desc.end = HWDesc.rp;
                Desc.wp = HWDesc.start;
                Desc.rp = HWDesc.end;
            }
            else {
                dbg_host_read8(ves_addr, (unsigned char *)&Desc, sizeof(struct AVStreamVESDesc_t));
            }

            if (Desc.wp >= Desc.rp) {
                flag_dir = 1;
            }
            else {
                flag_dir = 0;
            }

            if (flag_dir) {
                remain = Desc.wp - Desc.rp;
            }
            else {
                remain = (Desc.end - Desc.start + 1) - (Desc.rp - Desc.wp);
            }

            log(LOG_USER, "%s: %#X %#X %dMB %#X %#X, remain %dM %dK %dB, %s\n",
                flag_hardware_demux ? "HW" : "SW",
                Desc.start, Desc.end,
                (Desc.end - Desc.start + 1) >> 20,
                Desc.wp, Desc.rp,
                remain>>20 & 0x3FF,
                remain>>10 & 0x3FF,
                remain & 0x3FF,
                flag_dir ? "--->" : "<---");

            if (! flag_loop) {
                break;
            }

            Sleep(1000/100);
        }

        // DISABLE PMAN
    }

END:

    dbg_deinit();

    return 0;
}
