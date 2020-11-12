# PMAN

DCSN 网络，又叫 PMAN （目前这么理解）

在文件 ProSX8B_register_file_PMAN.xls 中，包括 4 大类寄存器，分别是 1040（UMAC），1010（DMA），1820（其他IP）和 PMAN Monitor。

* 1040 控制对 UMAC 的正常访问权限
* 1010 控制对 UMAC 的 DMA 访问权限
* 1820 是个 Hub 管理器，控制对其他 IP 的访问权限
* Monitor 管理 PMAN 并且如果有越界访问，这里会有相关记录

在文件 PMAN_DCSN_Architecture_for_SXs.pptx 中第 2 页，橙色块数字代表通道数量（推测），某些 IP 需要多个 DDR 通道，比如硬件解码器。

SX7B 和 SX8B 中有 3 个 UMAC

## TODO

* UMAC0/1/2 在 SX 上的使用情况

## 如何查看 AV/ARM/Malone 的 Security Number

    在 P111 平台：执行 cat /proc/pman_hub
    
        pman-hub regions:
        PManHub0 (reg0): 0x       0 - 0x bbff000 (0x   7108c, 0x   11101)
        PManHub0 (reg1): 0x bc00000 - 0x cbff000 (0x   40014, 0x   11101)
        PManHub0 (reg2): 0x cc00000 - 0x d7ff000 (0x 8000e06, 0x   11101)
        PManHub0 (reg3): 0x d800000 - 0x dfff000 (0x     222, 0x   11101)
        PManHub0 (reg4): 0x e000000 - 0x e7ff000 (0x 8000c06, 0x   11101)
        PManHub0 (reg5): 0x e000000 - 0x e7ff000 (0x       8, 0x   11001)
        PManHub0 (reg6): 0x e800000 - 0x e9ff000 (0x     e2e, 0x   11101)
        PManHub0 (reg7): 0x ea00000 - 0x ffff000 (0x  846c1e, 0x   11101)
        PManHub0 (reg8): 0x10000000 - 0x101ff000 (0x    600c, 0x   11101)
        PManHub0 (reg9): 0x10200000 - 0x10bff000 (0x    6004, 0x   11101)
        PManHub0 (reg10): 0x10c00000 - 0x117ff000 (0x    6e26, 0x   11101)
        PManHub0 (reg11): 0x10c00000 - 0x117ff000 (0x       8, 0x   11001)
        PManHub0 (reg12): 0x11800000 - 0x11cff000 (0x 1400016, 0x   11101)
        PManHub0 (reg13): 0x11800000 - 0x11cff000 (0x       8, 0x   11001)
        PManHub0 (reg14): 0x11d00000 - 0x1eaff000 (0x 178033e, 0x   11101)
        PManHub0 (reg15): 0x18f00000 - 0x1f4ff000 (0x2578019c, 0x   11101)
        PManHub0 (reg16): 0x1f500000 - 0x2d4ff000 (0x  80011c, 0x   11101)
        PManHub0 (reg17): 0x2d500000 - 0x395ff000 (0x     10c, 0x   11101)
        PManHub0 (reg18): 0x39600000 - 0x3ffff000 (0x   7108c, 0x   11101)
        PManHub0 (reg19): 0x40000000 - 0x7ffff000 (0x       0, 0x   11101)
        PManHub0 (reg20): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg21): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg22): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg23): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg24): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg25): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg26): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg27): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg28): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg29): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg30): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub0 (reg31): 0x       0 - 0x       0 (0x       0, 0x       0)

        PManHub1 (reg0): 0x80b00000 - 0x888ff000 (0x 178033e, 0x   11101)
        PManHub1 (reg1): 0x86700000 - 0x8f7ff000 (0x25f8019c, 0x   11101)
        PManHub1 (reg2): 0x8f800000 - 0xbffff000 (0x   7108c, 0x   11101)
        PManHub1 (reg3): 0xc0000000 - 0xfffff000 (0x       0, 0x   11101)
        PManHub1 (reg4): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg5): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg6): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg7): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg8): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg9): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg10): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg11): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg12): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg13): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg14): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub1 (reg15): 0x       0 - 0x       0 (0x       0, 0x       0)

        PManHub2 (reg0): 0x40000000 - 0x888ff000 (0x2178008c, 0x   11101)
        PManHub2 (reg1): 0x47300000 - 0x8f7ff000 (0x   7008c, 0x   11101)
        PManHub2 (reg2): 0x60000000 - 0xbffff000 (0x       0, 0x   11101)
        PManHub2 (reg3): 0x       0 - 0xfffff000 (0x       0, 0x   11101)
        PManHub2 (reg4): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg5): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg6): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg7): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg8): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg9): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg10): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg11): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg12): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg13): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg14): 0x       0 - 0x       0 (0x       0, 0x       0)
        PManHub2 (reg15): 0x       0 - 0x       0 (0x       0, 0x       0)

        -----------------------------------------------------
        PManSecurityGroup (No# 0): JTAG_MMIO
        PManSecurityGroup (No# 1): AV_CPU
        PManSecurityGroup (No# 2): PLF-ARMOR
        PManSecurityGroup (No# 3): PLF-ARM
        PManSecurityGroup (No# 4): DISP_CPU
        PManSecurityGroup (No# 5): MVED_CPU
        PManSecurityGroup (No# 6): ISDB-Demod
        PManSecurityGroup (No# 7): DMA_GATE
        PManSecurityGroup (No# 8): GFX_3D/GE2D/EGC2350
        PManSecurityGroup (No# 9): MALONE/HEVC/VP9
        PManSecurityGroup (No#10): DEMUX
        PManSecurityGroup (No#11): DEMUX-Read
        PManSecurityGroup (No#12): FLASH
        PManSecurityGroup (No#13): TURING
        PManSecurityGroup (No#14): TURING-Read
        PManSecurityGroup (No#15): ACP
        PManSecurityGroup (No#16): USB_1/USB_2/USB_3
        PManSecurityGroup (No#17): ETHERNET
        PManSecurityGroup (No#18): SDIO_1/2/SPI
        PManSecurityGroup (No#19): NR
        PManSecurityGroup (No#20): MCNR_W/MCNR_R1
        PManSecurityGroup (No#21): PIP_CAP
        PManSecurityGroup (No#22): MP_CAP
        PManSecurityGroup (No#23): OSD
        PManSecurityGroup (No#24): BL/PP_W/PP_R/PIP_CAP
        PManSecurityGroup (No#25): NONE
        PManSecurityGroup (No#26): TCD
        PManSecurityGroup (No#27): AUDIO
        PManSecurityGroup (No#28): NONE
        PManSecurityGroup (No#29): GME/2DD
        PManSecurityGroup (No#30): NONE
