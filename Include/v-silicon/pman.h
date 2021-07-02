#ifndef VS_PMAN_H
#define VS_PMAN_H

#define PMAN_WRITE_MASK         (1<<16)
#define PMAN_EXEC_MASK          (1<<12)
#define PMAN_READ_MASK          (1<<8)
#define PMAN_CONTENT_MASK       (1<<4)
#define PMAN_VALID_MASK         (1<<0)
#define PMAN_RW_MASK            (PMAN_VALID_MASK | PMAN_READ_MASK | PMAN_WRITE_MASK)

// SX7B P111
// #cat /proc/pman_hub
enum sx_security_group {
    PMAN_SECURITY_GROUP_ARM = 3,
};

#define PMAN_SECURITY_GROUP_JTAG                (0)
#define PMAN_SECURITY_GROUP_AVMIPS              (1)
#define PMAN_SECURITY_GROUP_ARMOR               (2)
#define PMAN_SECURITY_GROUP_ARM_M                 (3)
#define PMAN_SECURITY_GROUP_DISP                (4)
#define PMAN_SECURITY_GROUP_MVED                (5) //TODO: MVED VS. MALONE ?
#define PMAN_SECURITY_GROUP_ISDB_DEMOD          (6)
#define PMAN_SECURITY_GROUP_DMA                 (7)
#define PMAN_SECURITY_GROUP_GFX3D_GE2D_EGC2350  (8)
#define PMAN_SECURITY_GROUP_MALONE_HEVC_VP9     (9)
#define PMAN_SECURITY_GROUP_DEMUX               (10)
#define PMAN_SECURITY_GROUP_DEMUX_READ          (11)
#define PMAN_SECURITY_GROUP_FLASH               (12)
#define PMAN_SECURITY_GROUP_TURING              (13)
#define PMAN_SECURITY_GROUP_TURING_READ         (14)
#define PMAN_SECURITY_GROUP_ACP                 (15)
#define PMAN_SECURITY_GROUP_USB                 (16)
#define PMAN_SECURITY_GROUP_ETHERNET            (17)
#define PMAN_SECURITY_GROUP_SDIO_SPI            (18)
#define PMAN_SECURITY_GROUP_NR                  (19)
#define PMAN_SECURITY_GROUP_MCNR                (20)
#define PMAN_SECURITY_GROUP_PIP_CAP             (21)
#define PMAN_SECURITY_GROUP_MP_CAP              (22)
#define PMAN_SECURITY_GROUP_OSD                 (23)
#define PMAN_SECURITY_GROUP_BL_PP_PIP           (24)
#define PMAN_SECURITY_GROUP_NONE0               (25)
#define PMAN_SECURITY_GROUP_TCD                 (26)
#define PMAN_SECURITY_GROUP_AUDIO               (27)
#define PMAN_SECURITY_GROUP_NONE1               (28)
#define PMAN_SECURITY_GROUP_GME_2DD             (29)
#define PMAN_SECURITY_GROUP_NONE2               (30)

struct pman_ip_1040 {
    unsigned int addr_mask;
    unsigned int rsvd_00_20[3+4];
    unsigned int blocked_cmd;
    unsigned int blocked_cmd_id;
    unsigned int blocked_cmd_bs;
    unsigned int blocked_cmd_lines;
    unsigned int blocked_cmd_stride;
    unsigned int blocked_cmd_addr;
    unsigned int blocked_sec_group;
    unsigned int blocked_region;
    unsigned int rsvd_3c_60[4+4];
    unsigned int def_instruction_sec_access;
    unsigned int def_read_sec_access;
    unsigned int def_write_sec_access;
    unsigned int rsvd_68_100[1+4*9];
    struct region {
        unsigned int addr_low;
        unsigned int addr_high;
        unsigned int sec_access;
        unsigned int attr;
        unsigned int rsvd[4];
    } regions[32];
    unsigned int init_err_rd_data[6];
    unsigned int rsvd_514_FD8[2 + 4*14 + 4*16*9 + 4*13 + 2];
    unsigned int int_clr_enable;
    unsigned int int_set_enable;
    unsigned int int_status;
    unsigned int int_enable;
    unsigned int int_clr_status;
    unsigned int int_set_status;
    unsigned int rsvd_EC_FC[3];
    unsigned int module_id;
};

extern int vs_pman_get_block_info(unsigned int base, struct pman_ip_1040 *info);

#endif