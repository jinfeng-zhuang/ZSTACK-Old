#define ENABLE_LOG

#include <stdint.h>
#include <string.h>

#include <zstack/net.h>
#include <zstack/log.h>

#define VS_DEBUG_PORT (65528)

struct host_request {
    unsigned int head;
    unsigned int type;
    unsigned int addr;
    unsigned int count;
};

struct host_write_request {
    unsigned int head;
    unsigned int type;
    unsigned int addr;
    unsigned int count;
    unsigned char data[0x1000];
};

struct endian {
    unsigned int head;
    unsigned int little;
};

int dbg_init(const char* ip)
{
    int ret;
    struct endian request;
    short port = VS_DEBUG_PORT;

    log(LOG_USER, "Connecting...\n");

    ret = net_connect(ip, port, 0);

    if (ret != 0) {
        log(LOG_ERROR, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_USER, "Connected\n");

    request.head = 18 | ((sizeof(struct endian) - 4) << 16); // TODO
    request.little = 1;

    ret = net_transfer((unsigned char*)& request, NULL);

    if (-1 == ret) {
        log(LOG_WARNING, "%s set endian failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s set endian done\n", __FUNCTION__);

    return 0;
}

int dbg_deinit(void)
{
    net_disconnect();

    return 0;
}

int dbg_host_read32(unsigned int addr, unsigned int* buffer, int count)
{
    struct host_request request;
    int ret;

    if ((NULL == buffer) || (0 != (addr % 4)) || (count <= 0)) {
        log(LOG_WARNING, "%s param not correct\n", __FUNCTION__);
        return -1;
    }

    request.head = 16 | ((sizeof(struct host_request) - 4) << 16);
    request.type = 4;
    request.addr = addr;
    request.count = count;

    ret = net_transfer((unsigned char *)& request, buffer);

    if (-1 == ret) {
        log(LOG_WARNING, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;
}

int dbg_host_write32(unsigned int addr, unsigned int* buffer, int count)
{
    struct host_write_request request;
    int ret;

    if ((NULL == buffer) || (0 != (addr % 4)) || (count <= 0)) {
        log(LOG_WARNING, "%s param not correct\n", __FUNCTION__);
        return -1;
    }

    // TODO: host_write_request.data[4K] large
    request.head = 17 | ((sizeof(struct host_write_request) - 4) << 16);
    request.type = 4;
    request.addr = addr;
    request.count = count;
    memcpy(request.data, buffer, count * 4);

    ret = net_transfer((unsigned char *)& request, NULL);

    if (-1 == ret) {
        log(LOG_WARNING, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;
}

int dbg_avmips_read32(unsigned int addr, unsigned int* buffer, int count)
{
    struct host_request request;
    int ret;

    if ((NULL == buffer) || (((addr & 0xFFFFFF00) != 0xBADBAD00) && (0 != (addr % 4))) || (count <= 0)) {
        log(LOG_WARNING, "%s param not correct\n", __FUNCTION__);
        return -1;
    }

    request.head = 27 | ((sizeof(struct host_request) - 4) << 16);
    request.type = 4;
    request.addr = addr;
    request.count = count;

    ret = net_transfer((unsigned char*)& request, buffer);

    if (-1 == ret) {
        log(LOG_WARNING, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;
}

// debug server's mmap limit 64KB
static int dbg_host_read8_4K(unsigned int addr, unsigned char* buffer, int count)
{
    struct host_request request;
    int ret;

    if ((NULL == buffer) || (count <= 0)) {
        log(LOG_WARNING, "%s param not correct\n", __FUNCTION__);
        return -1;
    }

    // _DBGCMD_RD_HD_BULK_2_
    request.head = 16 | ((sizeof(struct host_request) - 4) << 16); // TODO 16 => Macro
    request.type = 1;
    request.addr = addr;
    request.count = count;

    ret = net_transfer((unsigned char*)& request, buffer);

    if (-1 == ret) {
        log(LOG_WARNING, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;
}

int dbg_host_read8(unsigned int addr, unsigned char* buffer, int count)
{
    int ret;
    unsigned int src_inc;
    unsigned char *dst_inc;
    unsigned int remain;
    unsigned int bytes2read;

    src_inc = addr;
    dst_inc = buffer;
    remain = count;

    log(LOG_DEBUG, "dbg_host_read8 %d...\n", count);

    memset(buffer, 0, count);

    while (remain > 0) {

        bytes2read = (remain > 0x1000) ? 0x1000 : remain;

        ret = dbg_host_read8_4K(src_inc, dst_inc, bytes2read);
        if (0 != ret) {
            log(LOG_WARNING, "dbg_host_read8 failed\n");
            goto END;
        }

        src_inc += bytes2read;
        dst_inc += bytes2read;

        remain = remain - bytes2read;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;

END:
    log(LOG_WARNING, "%s failed\n", __FUNCTION__);

    return -1;
}

// debug server's mmap limit 64KB
static int dbg_host_write8_4K(unsigned int addr, unsigned char* buffer, int count)
{
    struct host_write_request request;
    int ret;

    if ((NULL == buffer) || (count <= 0)) {
        log(LOG_WARNING, "%s param not correct\n", __FUNCTION__);
        return -1;
    }

    // _DBGCMD_WR_HD_BULK_2_
    request.head = 17 | ((sizeof(struct host_request) - 4 + count) << 16); // TODO 16 => Macro
    request.type = 1;
    request.addr = addr;
    request.count = count;
    memcpy(request.data, buffer, count);

    ret = net_transfer((unsigned char*)& request, NULL);

    if (-1 == ret) {
        log(LOG_WARNING, "%s failed\n", __FUNCTION__);
        return -1;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;
}

int dbg_host_write8(unsigned int addr, unsigned char* buffer, int count)
{
    int ret;
    unsigned int src_inc;
    unsigned char* dst_inc;
    unsigned int remain;
    unsigned int bytes2write;

    src_inc = addr;
    dst_inc = buffer;
    remain = count;

    log(LOG_DEBUG, "dbg_host_write8 %d...\n", count);

    while (remain > 0) {

        bytes2write = (remain > 0x1000) ? 0x1000 : remain;

        ret = dbg_host_write8_4K(src_inc, dst_inc, bytes2write);
        if (0 != ret) {
            log(LOG_WARNING, "dbg_host_write8 failed\n");
            goto END;
        }

        src_inc += bytes2write;
        dst_inc += bytes2write;

        remain = remain - bytes2write;
    }

    log(LOG_DEBUG, "%s done\n", __FUNCTION__);

    return 0;

END:
    log(LOG_WARNING, "%s failed\n", __FUNCTION__);

    return -1;
}

int dbg_avmips_read(unsigned int addr, unsigned char* buffer, unsigned int size)
{
    return 0;
}
