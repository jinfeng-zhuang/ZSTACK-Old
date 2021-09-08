#include <zstack/types.h>

void cloud_sync_thread(void) {
    u8 buffer[512];
    u32 ret;
    u32 cmd = 0;
    u32 id = 0;

    while (1) {

        // transfer to ID-TIME table, keep it simple for portable
        // if no, return 0;
        ret = zfs_get_table(buffer, sizeof(buffer));
        net_send(buffer, ret);

        // get the lastest data, if no, return 0;
        ret = zfs_get_data(-1, buffer, sizeof(buffer));
        net_send(buffer, ret);

        // cmd not only from server
        switch (cmd) {
            case 0: // SYNC CMD
                // return data size
                ret = zfs_get_data(id, buffer, sizeof(buffer));
                net_send(buffer, ret);
                break;
            default:
                break;
        }

        Sleep(500); // sample rate, can be configured
    }
}
