#include <zstack/zstack.h>
#include <v-silicon/v-silicon.h>

// SX7A, SX7B, SX8B use the same address for UMAC0/1/2, according to SPG
static unsigned int pman_umac_ctrl_base[3] = {
    0xF5005000, 0xF5008000, 0xF5036000
};

// enable [ip] access [address] with r/w/exec permission
int vs_pman_enable(enum sx_chip chip, enum sx_security_group group, unsigned int address)
{
    unsigned int regval = 0;
    int umac_idx = 0;
    int region_idx = 0;
    int ret;
    unsigned int addr_high;
    unsigned int addr_low;

    if ((chip != CHIP_SX7A) && (chip != CHIP_SX7B) && (chip != CHIP_SX8A) && (chip != CHIP_SX8B)) {
        warn("PMAN not supportedn");
        return -1;
    }

    // make sure the base address is correct and module is 1040
    ret = dbg_host_read32(offsetof(struct pman_ip_1040, module_id) + pman_umac_ctrl_base[umac_idx], &regval, 1);
    if (0 == ret) {
        if ((regval >> 16) != 0x1040) {
            warn("PMAN module ID = %#x, it should be 0x1040xxxx\n", regval);
            return -1;
        }
    }

    for (umac_idx = 0; umac_idx < 3; umac_idx++) {
        for(region_idx = 0; region_idx < 32; region_idx++)
        {
            ret = dbg_host_read32((u32)offsetof(struct pman_ip_1040, regions[region_idx].addr_low) + pman_umac_ctrl_base[umac_idx], &addr_low, 1);
            ret = dbg_host_read32((u32)offsetof(struct pman_ip_1040, regions[region_idx].addr_high) + pman_umac_ctrl_base[umac_idx], &addr_high, 1);
            info("UMAC[%d] PMAN Address [0x%08X 0x%08X]\n", umac_idx, addr_low, addr_high);
            if((0 ==ret) && (address >= addr_low) && (address <= addr_high)) {
                goto FOUND;
            }
        }
    }

    warn("pman address not found\n");
    return -1;

FOUND:
    info("address found in UMAC[%d] Region[%d]\n", umac_idx, region_idx);

    dbg_host_read32((u32)offsetof(struct pman_ip_1040, regions[region_idx].sec_access) + pman_umac_ctrl_base[umac_idx], &regval, 1);

    regval |= (1<<group);

    dbg_host_write32((u32)offsetof(struct pman_ip_1040, regions[region_idx].sec_access) + pman_umac_ctrl_base[umac_idx], &regval, 1);

    dbg_host_read32((u32)offsetof(struct pman_ip_1040, regions[region_idx].attr) + pman_umac_ctrl_base[umac_idx], &regval, 1);

    regval |= PMAN_RW_MASK;

    dbg_host_write32((u32)offsetof(struct pman_ip_1040, regions[region_idx].attr) + pman_umac_ctrl_base[umac_idx], &regval, 1);

    return 0;
}

int vs_pman_get_block_info(unsigned int base, struct pman_ip_1040 *info)
{
    int ret;

    if (NULL == info)
        return -1;

    ret = dbg_host_read32(
        base,
        (unsigned int *)info,
        (offsetof(struct pman_ip_1040, blocked_region) + 4) / 4);

    if  (ret != 0)
        return -1;

    return 0;
}
