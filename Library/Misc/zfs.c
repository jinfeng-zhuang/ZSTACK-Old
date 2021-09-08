#include <zstack/types.h>

/*
 * ===========
 * ZFS Design
 * ===========
 * 
 * | BadBlockMask | Statistic (BlockSize) | Data ---> | ... | <--- Table |
 * 
 * The info auto skip bad block.
 * 
 * Statistic:
 *	 Total Written
 * 
 * Table:
 *   ID | Timestamp | CRC | ADDRESS
 *     ID: Designed for sync with cloud
 *     TIMESTAMP:
 *     CRC:
 *     ADDRESS:
 */

#define BLOCK_SIZE (512)

static u64 wr;

static u8 tmpbuf[BLOCK_SIZE];
static u8 tmpbuf2[BLOCK_SIZE];

void zfs_init(void) {
    u64 total;

    total = flash_get_size();

    wr = total / 8 / BLOCK_SIZE + BLOCK_SIZE;
}

static u32 byte2block(u32 bytepos) {
    return bytepos / BLOCK_SIZE;
}

static u32 byte2offset(u32 bytepos) {
    return bytepos % BLOCK_SIZE;
}

static void block2bitmask(u32 bit, u32 *blk, u32 *byte, u32 *_bitpos) {
    u32 blkid, bytepos, bitpos;

    bytepos = bit / 8;
    bitpos  = bit % 8;
    blkid   = bytepos / BLOCK_SIZE;
    bytepos = bytepos % BLOCK_SIZE;

    *blk = blkid;
    *byte = bytepos;
    *_bitpos = bitpos;
}

u32 zfs_update_bad_block_mask(u32 blk) {
    u32 blkid, bytepos, bitpos;

    block2bitmask(blk, &blkid, &bytepos, &bitpos);

    flash_read(tmpbuf, blkid);
    tmpbuf[bytepos] |= 1 << (7 - bitpos);
    flash_write(tmpbuf, blkid);
}

u32 zfs_is_bad_block(u32 blk) {
    u32 blkid, bytepos, bitpos;

    block2bitmask(blk, &blkid, &bytepos, &bitpos);

    flash_read(tmpbuf, blkid);
    if (tmpbuf[bytepos] & 1 << (7 - bitpos)) {
        return 1;
    } else {
        return 0;
    }
}

// usage: while(writeblock ok)
u32 zfs_write_block(u8* data, u32 offset, u32 datalen, u32 *blkid) {
    if (zfs_is_bad_block(*blkid)) {
        *blkid = *blkid + 1;

        return -1;
    }

    flash_read(tmpbuf, blkid);
    memcpy(tmpbuf, offset, datalen);
    flash_write(tmpbuf, blkid);
    flash_read(tmpbuf2, blkid);
    if (0 != memcmp(tmpbuf, tmpbuf2, BLOCK_SIZE)) {
        zfs_update_bad_block_mask(blkid);
        *blkid = *blkid + 1;
        return -1;
    } else {
        return 0;
    }
}

// return -1: failed
// return 0: ok
u32 zfs_store(u8* buffer, u32 length) {
    u64 wr;
    u32 remain;
    u32 size1;
    u32 size2;
    u32 i;

    // TODO wr update internal
    wr = zfs_get_wr();

    remain = BLOCK_SIZE - wr & (BLOCK_SIZE - 1);
    if (remain >= length) {
        size1 = length;
    } else {
        size1 = remain;
        size2 = length - remain;
    }

    // write to first part
    while (0 != zfs_write_block(buffer, byte2offset(wr), size1, byte2block(wr)));

    wr += size1;

    for (i = 0; i < size2 / BLOCK_SIZE; i++) {
        while (0 != zfs_write_block(buffer, byte2offset(wr), size1, byte2block(wr)));
    }

    return 0;
}

// return -1: failed, else, crc value
u32 zfs_load(u8* buffer, u32 length) {
    return 0;
}

enum {
    ZFS_CMD_READ,
    ZFS_CMD_START,
};

u32 zfs_event_handler(u32 cmd, u32 param) {

}

#define ZFS_ITEM_SIZE (32) // fixed for not-cross-block condition
#define ZFS_BLOCK_ITEM_NUM (BLOCK_SIZE / ZFS_ITEM_SIZE) // = 16, fixed

struct zfs_item {
    u32 id;         // for continuous and locate directly
    u32 rsvd;       // rsvd
    u64 rsvd2;      // rsvd
    u64 timestamp;  // for search based on time
    u64 address;    // for locate data directly
};

struct zfs_info {
    u64 data_wr;
    u64 item_wr;
    u32 block_total;
};

u32 zfs_get_data(u32 id, u8* buffer, u32 length) {
    u32 item_blk;
    u32 item_offset;
    u64 address;
    struct zfs_item *item;

    item_blk = id / ZFS_BLOCK_ITEM_NUM;
    item_offset = (id % ZFS_BLOCK_ITEM_NUM);

    // TODO: check blk bad, skip it auto

    // TODO block = total - block;

    flash_read(tmpbuf, item_blk);

    if (id != ((struct zfs_item *)tmpbuf)[item_offset].id) {
        return -1;
    }

    address = ((struct zfs_item *)tmpbuf)[item_offset].address;

    // read header

    // check buffer length if valid

    // copy to buffer

    // return 0;
}
