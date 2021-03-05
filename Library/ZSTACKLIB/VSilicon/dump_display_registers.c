#define ENABLE_LOG

#include <vs/avmips.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include <vs/dbg.h>

int read_mpegdisp_regsiters(unsigned int base, struct mpegdisp_regs *regs)
{
    int ret;
    static unsigned int channel_1_field_interrupt_ro_prev = 0;

    if (regs == NULL) {
        return -1;
    }

    ret = dbg_host_read32(base | 0xF0000000, (unsigned int *)regs, sizeof(struct mpegdisp_regs) / sizeof(unsigned int));
    if (0 == ret) {
        if (channel_1_field_interrupt_ro_prev != regs->channel_1_field_interrupt_ro) {
            channel_1_field_interrupt_ro_prev = regs->channel_1_field_interrupt_ro;
            return 0;
        }
    }
    
    return -1;
}

int mpegdisp_is_resolution_change(struct mpegdisp_regs *regs)
{
    static unsigned int res_prev = 0;

    if (res_prev != regs->resolution_change) {
        res_prev = regs->resolution_change;
        return true;
    }
    
    return false;
}

void dump_mpegdisp_registers(struct mpegdisp_regs *regs)
{
    if (0 == regs) {
        return;
    }

    log_info(
        "Mode %s, ToSVP %s, Vsync %s, Interrupt %s, bluescreen %d\n"
        "Resolution Change %d Int %d, %dx%d\n"
        "Y: %#x %#x %#x %#x\n"
        "C: %#x %#x %#x %#x\n",
        regs->channel_1_field_interrupt_ro & 0x1 ? "interface" : "progressive",
        regs->channel_1_field_interrupt_ro & 0x2 ? "bottom" : "top",
        regs->channel_1_field_interrupt_ro & 0x4 ? "bottom" : "top",
        regs->channel_1_field_interrupt_ro & 0x8 ? "bottom" : "top",
        regs->blue_screen,
        regs->resolution_change,
        regs->int_resolution_change,
        regs->h_width, regs->h_height,
        regs->addr_y[0], regs->addr_y[1], regs->addr_y[2], regs->addr_y[3],
        regs->addr_c[0], regs->addr_c[1], regs->addr_c[2], regs->addr_c[3]
        );

    return;
}
