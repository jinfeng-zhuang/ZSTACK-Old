#ifndef SX_H
#define SX_H

#include <vs/pman.h>

///////////////////////////////////////////////////////////////////////////////

#define SX_REG_BASE (0x10000000)

///////////////////////////////////////////////////////////////////////////////

#define MPEGDISP_REG_BASE   (SX_REG_BASE + 0x13000)

#define MPEGDISP_Y_C_EVEN (MPEGDISP_REG_BASE + 0x30)
#define MPEGDISP_Y_P1_EVEN (MPEGDISP_REG_BASE + 0x34)
#define MPEGDISP_Y_P2_EVEN (MPEGDISP_REG_BASE + 0x38)
#define MPEGDISP_Y_P3_EVEN (MPEGDISP_REG_BASE + 0x3c)
#define MPEGDISP_C_C_EVEN (MPEGDISP_REG_BASE + 0x40)
#define MPEGDISP_C_P1_EVEN (MPEGDISP_REG_BASE + 0x44)
#define MPEGDISP_C_P2_EVEN (MPEGDISP_REG_BASE + 0x48)
#define MPEGDISP_C_P3_EVEN (MPEGDISP_REG_BASE + 0x4c)

#define MPEGDISP_CH1_C0_FRMINFO_SX7 (0x19603DC0)
#define MPEGDISP_CH1_P1_FRMINFO_SX7 (0x19603DC4)
#define MPEGDISP_CH1_P2_FRMINFO_SX7 (0x19603DC8)
#define MPEGDISP_CH1_P3_FRMINFO_SX7 (0x19603DCC)
#define MPEGDISP_CH2_C0_FRMINFO_SX7 (0x19603DD0)
#define MPEGDISP_CH2_P1_FRMINFO_SX7 (0x19603DD4)
#define MPEGDISP_CH2_P2_FRMINFO_SX7 (0x19603DD8)
#define MPEGDISP_CH2_P3_FRMINFO_SX7 (0x19603DDC)

#define MPEGDISP_CH1_SEAMLESS_FRMINFO_SX7 (0x19603DE0)
#define MPEGDISP_CH2_SEAMLESS_FRMINFO_SX7 (0x19603DE4)

#define MPEGDISP_CH1_C0_FRMINFO_SX8 (0x196809D8)
#define MPEGDISP_CH1_P1_FRMINFO_SX8 (0x196809DC)
#define MPEGDISP_CH1_P2_FRMINFO_SX8 (0x196809E0)
#define MPEGDISP_CH1_P3_FRMINFO_SX8 (0x196809E4)
#define MPEGDISP_CH2_C0_FRMINFO_SX8 (0x196809E8)
#define MPEGDISP_CH2_P1_FRMINFO_SX8 (0x196809EC)
#define MPEGDISP_CH2_P2_FRMINFO_SX8 (0x196809F0)
#define MPEGDISP_CH2_P3_FRMINFO_SX8 (0x196809F4)

#define MPEGDISP_CH1_SEAMLESS_FRMINFO_SX8 (0x196809F8)
#define MPEGDISP_CH2_SEAMLESS_FRMINFO_SX8 (0x196809FC)

///////////////////////////////////////////////////////////////////////////////

//SX6-SX7 PMAN Setting Reg 
#define UMAC0_PMAN_REG_BASE 0xF5005100
#define UMAC1_PMAN_REG_BASE 0xF5008100

///////////////////////////////////////////////////////////////////////////////

enum sx_chip {
    CHIP_UNKNOWN,
    CHIP_SX7A,
    CHIP_SX7B,
    CHIP_SX8A,
    CHIP_SX8B,
    CHIP_Union1A,
    CHIP_Union1B,
    CHIP_Union2A,
    CHIP_Union2B
};

///////////////////////////////////////////////////////////////////////////////

extern int vs_chip_id_get(void);

extern int vs_pman_enable(enum sx_chip chip, enum sx_security_group ip, unsigned int address);

#endif